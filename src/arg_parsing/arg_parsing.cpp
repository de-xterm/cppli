
#include <stdexcept>
#include <cassert>
#include <iostream>

#include "arg_parsing.h"

namespace cppli::detail {
    std::vector<subcommand_t> parse(int argc, const char* const* const argv) {

        if(argc == 0) {
            std::cerr << "argc == 0. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }
        if(!argv[0]) {
            std::cerr << "argv[0] was null. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }

        // skip the program name
        std::vector<subcommand_t> commands;

        subcommand_name_t subcommand_name/* = {{argv[0]}}*/;

        subcommand_inputs_t args;

        bool disambiguate_next_arg = false;

        bool first_command = true;
        std::string first_command_name = argv[0];

        bool in_namespace = main_command_is_namespace();
        commands.push_back({{{argv[0]}}, args});

        std::string command_or_subcommand = "command";

        std::string current_subcommand_name_string = argv[0];


        for(unsigned arg_i = 1; arg_i < argc; ++arg_i) { // TODO: message when ignoring unrecognized options/flags/args
            std::string arg_string = argv[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string) && !disambiguate_next_arg) {
                in_namespace = is_namespace(subcommand_name);

                command_or_subcommand = "subcommand";
                current_subcommand_name_string = arg_string;

                commands.push_back({subcommand_name, args});

                args = {};
            }
            else {
                if(in_namespace) {
                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                    continue;
                }
                if((arg_string.substr(0,2) == "--") && !disambiguate_next_arg) { // long flag (these are ez)
                                                    // we need this check so that we can handle the case where "--" is the thing we're trying to disambiguate. Ex: "program -- --"
                    if((arg_string.size() == 2) /*&& (!disambiguate_next_arg)*/) { // if the whole string is just "--", then this arg is used to disambiguate the next
                        disambiguate_next_arg = true; // ( "--" just means "the next arg is positional, even if it looks like an option/flag (starts with '-' or "--"), or a subcommand (matches a subcommand name))
                    }
                    else {
                        std::string::size_type equals_pos;
                        if((equals_pos = arg_string.find('=')) != std::string::npos) { // TODO: make sure the -2 doesn't cause issues
                            std::string option_name  = arg_string.substr(2, equals_pos-2),
                                    option_value = arg_string.substr(equals_pos+1, arg_string.size()-(equals_pos+1));

                            if(subcommand_takes_option(subcommand_name, option_name)) {
                                args.options_to_values.emplace(option_name, option_value);
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_name)) {
                                std::cerr << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << option_name << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \"" << arg_string << "\"). "
                                             "The value will be ignored and the flag will be set to true\n";

                                args.flags.emplace(option_name);
                            }
                            else {
                                std::cerr << "Flag/option \"" << option_name << "\" (from \"" << arg_string << "\") was not recognized by " << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and is therefore being ignored\n";
                            }
                        }
                        else {
                            std::string option_or_flag_name = arg_string.substr(2, arg_string.size()-1);

                            if(subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                                if(subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                                    args.options_to_values.emplace(option_or_flag_name, std::nullopt);
                                }
                                else {
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(option_or_flag_name, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        throw std::runtime_error('\"' + arg_string + "\" referred to an option with a required argument, but no argument followed\n");
                                    }
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_or_flag_name)) {
                                args.flags.emplace(option_or_flag_name);
                            }
                            else {
                                throw std::runtime_error(command_or_subcommand + " \"" + current_subcommand_name_string + "\" does not accept a flag or option \"" + arg_string + '\"');
                            }
                        }
                    }
                }
                else if((arg_string[0] == '-') && !disambiguate_next_arg) { // short flag(s) and/or option (these are not so ez)
                    if(arg_string.find('=') != std::string::npos) {
                        for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                            std::string char_string = arg_string.substr(char_i,1);
                            if((char_i < arg_string.size()-1) && (arg_string[char_i+1] == '=')) {
                                if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, arg_string.substr(char_i+2, arg_string.size()-(char_i+2)));
                                }
                                else if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    std::cerr << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << char_string << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in flag/option group \"" << arg_string << "\"). "
                                                 "The value will be ignored and the flag will be set to true\n";

                                    args.flags.emplace(char_string);
                                }
                                else {
                                    std::cerr << "Flag/option \"" << char_string << "\" (from flag/option group \"" << arg_string << "\") was not recognized by" << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and is therefore being ignored\n";
                                }
                                break;
                            }
                            else {
                                if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    args.flags.emplace(char_string);
                                }
                                else if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, std::nullopt);
                                }
                                else {
                                    std::cerr << "Flag/option \"" << char_string << "\" (from flag/option group \"" << arg_string << "\") was not recognized by" << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and is therefore being ignored\n";
                                }
                            }
                        }
                    }
                    else {
                        for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                            std::string char_string = arg_string.substr(char_i,1);

                            if(subcommand_takes_option(subcommand_name, char_string)) {
                                if(char_i < arg_string.size()-1) { // no equals sign, so everything after this character is the argument    // TODO: maybe check to see if the string contains any flags with required args, and then if not we can employ this logic
                                    args.options_to_values.emplace(char_string, arg_string.substr(char_i+1, arg_string.size()-(char_i+1)));
                                    break;
                                }
                                else if(char_i == arg_string.size()-1) {
                                    if(subcommand_option_argument_is_optional(subcommand_name, char_string)) { // if this option's argument is optional, assume that no argument is provided, and that the next arg is unrelated
                                        args.options_to_values.emplace(char_string, std::nullopt);
                                    }
                                    else {
                                        if(arg_i+1 < argc) {
                                            args.options_to_values.emplace(char_string, std::string(argv[arg_i+1]));
                                            ++arg_i; // we just ate the next arg, so don't process it again
                                        }
                                        else {
                                            throw std::runtime_error(std::string("The last character (") + arg_string[char_i] + ") in flag/option group \"" + arg_string + "\" referred to an option with a required argument, but no argument followed\n");
                                        }
                                    }
                                }
                                else {
                                    assert(false); // this should never happen
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, char_string)) {
                                args.flags.emplace(char_string);
                            }
                            else {
                                std::cerr << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" " // TODO: can't I make this nonfatal?
                                                         "did not form a valid flag or option for " << command_or_subcommand << " \"" << current_subcommand_name_string << "\"\n";
                            }
                        }
                    }
                }
                else { // positional arg
                    disambiguate_next_arg = false;
                    args.positional_args.emplace_back(std::move(arg_string));
                }
            }
        }

        //if(commands.size() > 0) {
            commands.back().inputs = std::move(args);
        //}

        return std::move(commands);
    }
}