
#include <cassert>
#include <iostream>
#include <sstream>
#include <format>

#include "arg_parsing.h"
#include "documentation.h"
#include "configuration.h"
#include "iro.h"

namespace cppli::detail {
    bool contains_letters(const std::string& str) {
        bool contains = false;
        for(char c : str) {
            contains |= isletter(c);
        }

        return contains;
    }

    std::string say_something_if_empty(const std::string& arg) {
        if(!arg.size()) {
            return "(empty string)";
        }
        else {
            return '\"' + arg + '\"';
        }
    }

    // TODO: break each major if in the loop into a function, and put each function in its own file
    parse_ret_t parse(int argc, const char* const* const argv) {

        if(argc == 0) {
            std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argc == 0. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }
        if(!argv[0]) {
            std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argv[0] was null. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }

        /*if(program_name() != argv[0]) { // TODO: switch to enable/disable this warning. I'm commenting it out for now because it's annoying
            std::cerr << "The name used to invoke the program (\"" << argv[0] << "\") was not the same as the program's documented name (\"" << program_name() << "\")\n";
        }*/

        // skip the program name
        command_name_t subcommand_name = {"MAIN"};

        //command_inputs_t args;
        std::vector<command_t> commands{{subcommand_name}};

        bool disambiguate_next_arg = false;
        bool disambiguate_until_subcommand = false;
        bool disambiguate_all = false;

        bool invalid_input_to_namespace = false;
                                                    // possibly the longest variable name I've ever written
        std::unordered_map<unsigned, std::string> optional_argument_option_with_no_value_provided_arg_index_to_option_string; // indices in argv of optional argument optionals that weren't provided an argument
                                                                                                                              // used to make better error messages
        std::string first_command_name = argv[0];

        bool in_namespace = is_namespace({"MAIN"});

        std::optional<unsigned> help_command_index;

        std::string command_or_subcommand = "main command";

        std::string current_subcommand_name_string = to_string(subcommand_name);

        parse_ret_t ret;

        for(unsigned arg_i = 1; arg_i < argc; ++arg_i) {
            std::string arg_string = argv[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string) && !(disambiguate_next_arg || disambiguate_all)) {
                if(arg_string == "help") {
                    ret.help_command_index = commands.size();
                }
                in_namespace = is_namespace(subcommand_name);

                command_or_subcommand = "subcommand";
                current_subcommand_name_string = to_string(subcommand_name);

                disambiguate_until_subcommand = false;

                //commands.back().inputs = std::move(args);
                //args = {};

                commands.push_back({subcommand_name});
            }
            else {
                command_inputs_t& args = commands.back().inputs;
                /*if(in_namespace) { // I forgot why this doesn't work
                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                    continue;
                }*/
                if((arg_string.substr(0,2) == "--") && !(disambiguate_next_arg || disambiguate_until_subcommand || disambiguate_all)) { // long flag (these are ez)
                                                    // we need this check so that we can handle the case where "--" is the thing we're trying to disambiguate. Ex: "program -- --"
                    if(arg_string[2] == '1') { // arg_string == "--1"
                        disambiguate_next_arg = true;
                    }
                    else if(arg_string.size() == 2) { // arg_string == "--"
                        disambiguate_until_subcommand = true;
                    }
                    else if(arg_string[2] == '-') { // arg_string == "---"
                        disambiguate_all = true;
                    }
                    else {
                        std::string::size_type equals_pos;
                        if((equals_pos = arg_string.find('=')) != std::string::npos) { // TODO: make sure the -2 doesn't cause issues
                            std::string option_name  = arg_string.substr(2, equals_pos-2),
                                        option_value = arg_string.substr(equals_pos+1, arg_string.size()-(equals_pos+1));

                            if(subcommand_takes_option(subcommand_name, option_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_name);
                                args.options_to_values.emplace(option_name, option_value);
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_name)) {
                                std::stringstream ss;
                                ss << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << option_name << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \"" << arg_string << "\"). ";

                                print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(), "The value will be ignored and the flag will be set to true\n\n");

                                args.flags.emplace(option_name);
                            }
                            else {
                                if(option_name == "help") {
                                    print_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::stringstream ss;
                                    ss << '\"' << current_subcommand_name_string << "\" is a namespace, so it cannot accept the input \"" << arg_string << "\" (the only inputs it can accept are --help, -h, and help)";

                                    print_throw_or_do_nothing(ARGUMENT_GIVEN_TO_NAMESPACE, ss.str(), "The given input be ignored\n\n");
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    std::stringstream ss;
                                    ss << "Flag/option \"" << option_name << "\" (from \"" << arg_string << "\") was not recognized by " << command_or_subcommand << " \"" << current_subcommand_name_string << "\"\n";

                                    print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), "It will be ignored\n\n");
                                }
                            }
                        }
                        else {
                            std::string option_or_flag_name = arg_string.substr(2, arg_string.size()-1);

                            {
                                bool all_letter_or_hyphen = true;
                                for(char c : option_or_flag_name) {
                                    all_letter_or_hyphen &= (isletter(c) || (c == '-'));
                                }

                                if(!all_letter_or_hyphen) {
                                    // TODO: rename INVALID_FLAG to be more general (because technically we don't konw if it's a flag or an option)
                                    print_throw_or_do_nothing(INVALID_FLAG, std::format("Flag or option \"{}\" contains characters other than letters and hyphens, and therefore cannot form a valid flag/option", option_or_flag_name), ". It will be ignored\n\n");
                                    continue;
                                }
                            }

                            //error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                            if(subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                                if(subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                                    args.options_to_values.emplace(option_or_flag_name, std::nullopt);
                                    optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i, arg_string);
                                }
                                else {
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(option_or_flag_name, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        std::stringstream ss;
                                        ss << "Argument \"" << arg_string
                                           << "\" given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                                           << " referred to an option with a required argument, but no argument followed\n";

                                        throw user_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                    }
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_or_flag_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);
                                args.flags.emplace(option_or_flag_name);
                            }
                            else {
                                if(option_or_flag_name == "help") {
                                    print_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::cerr << iro::bright_yellow << iro::effect_string(iro::bold|iro::underlined, "Warning:") <<  " \"" << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    print_throw_or_do_nothing(INVALID_FLAG, std::format(R"({} "{}" does not accept a flag or option "{}")", command_or_subcommand, current_subcommand_name_string, arg_string), " It will be ignored\n\n");
                                }
                            }
                        }
                    }
                }                                  // so that string like "-" and " - " can be used as positionals without issue
                else if((arg_string[0] == '-') && (arg_string.size() > 1) && isletter(arg_string[1]) && !(disambiguate_next_arg || disambiguate_until_subcommand || disambiguate_all)) { // short flag(s) and/or option (these are not so ez)
                    bool invalid_character_in_flag_group = false;            // don't consider this arg a flag group if it starts with an invalid char (not a letter)
                    unsigned invalid_character_index;                        // allows the user to enter negative numbers as positional arguments with a positional separator
                    std::stringstream invalid_character_in_flag_group_message;

                    for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                        std::string char_string = arg_string.substr(char_i,1);

                        if(subcommand_takes_option(subcommand_name, char_string)) { // there is an argument
                            error_if_short_flag_or_option_already_included(commands.back(), char_string);
                            if(char_i < arg_string.size()-1) { // no equals sign, so everything after this character is the argument    // TODO: maybe check to see if the string contains any flags with required args, and then if not we can employ this logic
                                args.options_to_values.emplace(char_string, arg_string.substr(char_i+1+(arg_string[char_i+1] == '='), arg_string.size()));
                                break;                                                                 // ^ discard a leading '='!!
                            }
                            else if(char_i == arg_string.size()-1) { // no room for argument
                                if(subcommand_option_argument_is_optional(subcommand_name, char_string)) { // if this option's argument is optional, assume that no argument is provided, and that the next arg is unrelated
                                    args.options_to_values.emplace(char_string, std::nullopt);
                                    optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i, "-" + char_string);
                                }
                                else {
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(char_string, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        std::stringstream ss;
                                        ss << "The last character '" << arg_string[char_i]
                                           << "' in flag/option group \"" << arg_string << "\" "
                                              "given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                                           << " referred to an option with a required argument, but no argument followed\n";

                                        throw user_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                    }
                                }
                            }
                            else {
                                assert(false); // this should never happen (would mean char_i is out of range of arg_string)
                            }
                        }
                        else if(subcommand_takes_flag(subcommand_name, char_string)) {
                            error_if_short_flag_or_option_already_included(commands.back(), char_string);
                            args.flags.emplace(char_string);
                        }
                        else if(char_string == "h") {
                            print_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                            return {{}, true};
                        }
                        else if(in_namespace) {
                            std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                            invalid_input_to_namespace = true;
                            continue;
                        }
                        else {
                            if(!isletter(char_string[0])) {
                                if(!invalid_character_in_flag_group) {
                                    invalid_character_index = char_i; // because we only want to set this the first time we find an invalid character
                                }
                                invalid_character_in_flag_group = true;
                                invalid_character_in_flag_group_message << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" "
                                   << "given to " << command_or_subcommand << " \"" << current_subcommand_name_string << "\" "
                                   << "is not a letter and therefore cannot form a valid short name for a flag or option\n";

                                if(arg_string[invalid_character_index] == '=') {
                                    break; // so that we don't continue to look for flags (valid or invalid) in the "argument"
                                }
                            }
                            else {
                                std::stringstream ss;
                                ss << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" "
                                      "was not a recognized flag or option for " << command_or_subcommand << " \"" << current_subcommand_name_string << "\".";

                                print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), " It will be ignored\n\n");
                            }
                        }
                    }

                    if(invalid_character_in_flag_group) {
                        if((invalid_character_index > 1) && subcommand_takes_flag(subcommand_name, {arg_string[invalid_character_index-1]})) { // 1 instead of 0 because of the leading '-'
                            if(arg_string[invalid_character_index] == '=') {
                                std::stringstream ss;
                                ss << "Character '" << arg_string[invalid_character_index-1] << "' in flag/option group \"" << arg_string << "\" is a flag, and therefore can't accept an argument.\n";

                                print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(),
                                                          "The argument " + say_something_if_empty(arg_string.substr(invalid_character_index+1, arg_string.size())) +
                                                          " will be ignored and the flag '" + arg_string[invalid_character_index-1] + "' will be set to true\n");
                                continue;
                            }
                            else {
                                invalid_character_in_flag_group_message << "It's possible that the substring \"" << arg_string.substr(invalid_character_index, arg_string.size())
                                                                        << "\" was meant to be an argument for '" << arg_string[invalid_character_index-1] << "', but '"
                                                                        << arg_string[invalid_character_index-1] << "' is flag and therefore can't have an argument.\n";
                            }
                        }
                        print_throw_or_do_nothing(INVALID_FLAG, invalid_character_in_flag_group_message.str(), "Invalid flag(s) will be ignored\n\n");
                    }

                }
                else { // positional arg
                    if(in_namespace) {
                        std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                        invalid_input_to_namespace = true;
                        continue;
                    }
                    else {
                        disambiguate_next_arg = false;
                        args.positional_args.emplace_back(arg_string);
                    }

                    const auto& docs = subcommand_name_to_docs()[commands.back().name];
                    const auto& command = commands.back();

                    unsigned expected_positionals_count = docs.positionals.size(),
                             actual_positionals_count   = args.positional_args.size();

                    if(!docs.variadic && (actual_positionals_count > expected_positionals_count)) {

                        std::stringstream ss;
                        ss << "Unexpected positional argument \"" << arg_string
                           << "\" given to " << (command.name == command_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(command.name) << '\n';/* <<
                           "\" (expected " << expected_positionals_count << ", got " << actual_positionals_count << ").\n";*/

                        ss << "It's also possible that \"" << arg_string
                           << "\" was supposed to be a subcommand, but was not recognized. "
                              "You can run\n" << to_string(command.name, " ") << " help\nto view the available subcommands for this command\n";

                        minor_error_type e;
                        if(optional_argument_option_with_no_value_provided_arg_index_to_option_string.contains(arg_i-1)) {
                            const std::string& erroneous_option_string = optional_argument_option_with_no_value_provided_arg_index_to_option_string.at(arg_i-1);
                            ss << "\nAnother possibility is that \"" << arg_string << "\" was supposed to be an argument for the previous commandline argument "
                               << '\"' << optional_argument_option_with_no_value_provided_arg_index_to_option_string.at(arg_i - 1) << '\"'
                               << ", which is an option that accepts an optional argument.\n"
                                  "However, an optional argument option can't be given an argument using the space separated syntax.\n"
                                  "The argument must use the '=' syntax (" << erroneous_option_string << '=' << arg_string << ')';

                            if(std::string(argv[arg_i-1]).substr(0,2) != "--") {
                                ss << " or, for (potentially grouped) short options, the connected syntax ("
                                   << erroneous_option_string << arg_string << ")";
                            }

                            ss << '\n';

                            e = EXCESS_POSITIONAL_WITH_SUSPICIOUS_OPTION;
                        }
                        else {
                            e = EXCESS_POSITIONAL;
                        }


                        print_throw_or_do_nothing(e, ss.str(), "This argument will be ignored\n\n");
                    }
                }
            }
        }

        if(is_namespace(commands.back().name)) {
            /*if(invalid_input_to_namespace) {
                std::cout << "Here is its help page:\n";
            }
            else {*/
            if(!invalid_input_to_namespace) { // TODO: make it configurable whether invalid input to namespace just gives a warning, or causes the program to stop and print help
                std::cout << '\"' << to_string(commands.back().name)
                          << "\" is a namespace, so using it without further subcommands doesn't do anything. Here is its help page: \n";
                //}
                print_documentation_string_callback(commands.back().name, default_top_level_help_verbosity, default_subcommand_help_verbosity,
                                                                 default_help_recursion_level, default_hide_help_status);
                return {{}, true};
            }
        }

        ret.subcommands = std::move(commands);
        return ret;
    }
}