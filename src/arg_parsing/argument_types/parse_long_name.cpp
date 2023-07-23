
#include <sstream>

#include "subcommand.h"
#include "configuration.h"
#include "documentation.h"
#include "arg_parsing.h"

namespace cppli::detail {
    std::optional<parse_ret_t> parse_long_name(const std::string& arg_string,
                         unsigned arg_i,
                         int argc, int argv,
                         bool& disambiguate_next_arg,
                         const subcommand_name_t& subcommand_name, const std::vector<subcommand_t>& commands,
                         const std::string& command_or_subcommand, subcommand_inputs_t& args,
                         const std::string& current_subcommand_name_string,
                         bool& invalid_input_to_namespace, bool in_namespace,
                         std::unordered_map<unsigned, std::string>& optional_argument_option_with_no_value_provided_arg_index_to_option_string) {
        // we need this check so that we can handle the case where "--" is the thing we're trying to disambiguate. Ex: "program -- --"
        if ((arg_string.size() ==2) /*&& (!disambiguate_next_arg)*/) { // if the whole string is just "--", then this arg is used to disambiguate the next
            disambiguate_next_arg = true; // ( "--" just means "the next arg is positional, even if it looks like an option/flag (starts with '-' or "--"), or a subcommand (matches a subcommand name))
        }
        else {
            std::string::size_type equals_pos;
            if ((equals_pos = arg_string.find('=')) !=
                std::string::npos) { // TODO: make sure the -2 doesn't cause issues
                std::string option_name = arg_string.substr(2, equals_pos - 2),
                        option_value = arg_string.substr(equals_pos + 1, arg_string.size() - (equals_pos + 1));

                if (subcommand_takes_option(subcommand_name, option_name)) {
                    error_if_flag_or_option_already_included(commands.back(), option_name);
                    args.options_to_values.emplace(option_name, option_value);
                } else if (subcommand_takes_flag(subcommand_name, option_name)) {
                    std::stringstream ss;
                    ss << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \""
                       << option_name
                       << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \""
                       << arg_string << "\"). ";

                    print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(),
                                              "The value will be ignored and the flag will be set to true\n");

                    args.flags.emplace(option_name);
                } else {
                    if (option_name == "help") {
                        std::cout << (get_documentation_string_callback())(subcommand_name, default_help_verbosity,
                                                                           default_help_recursion_level,
                                                                           default_hide_help_status);
                        return parse_ret_t{{}, true};
                    } else if (in_namespace) {
                        std::stringstream ss;
                        ss << '\"' << current_subcommand_name_string
                           << "\" is a namespace, so it cannot accept the input \"" << arg_string
                           << "\" (the only inputs it can accept are --help, -h, and help)";

                        print_throw_or_do_nothing(ARGUMENT_GIVEN_TO_NAMESPACE, ss.str(),
                                                  "The given input be ignored\n");
                        invalid_input_to_namespace = true;
                        continue;
                    } else {
                        std::stringstream ss;
                        ss << "Flag/option \"" << option_name << "\" (from \"" << arg_string
                           << "\") was not recognized by " << command_or_subcommand << " \""
                           << current_subcommand_name_string << "\"\n";

                        print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), "It will be ignored\n");
                    }
                }
            } else {
                std::string option_or_flag_name = arg_string.substr(2, arg_string.size() - 1);

                //error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                if (subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                    error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                    if (subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                        args.options_to_values.emplace(option_or_flag_name, std::nullopt);
                        optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i,
                                                                                                           arg_string);
                    } else {
                        if (arg_i + 1 < argc) {
                            args.options_to_values.emplace(option_or_flag_name, std::string(argv[arg_i + 1]));
                            ++arg_i; // we just ate the next arg, so don't process it again
                        } else {
                            std::stringstream ss;
                            ss << "Argument \"" << arg_string
                               << "\" given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                               << " referred to an option with a required argument, but no argument followed\n";

                            throw user_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                        }
                    }
                } else if (subcommand_takes_flag(subcommand_name, option_or_flag_name)) {
                    error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);
                    args.flags.emplace(option_or_flag_name);
                } else {
                    if (option_or_flag_name == "help") {
                        std::cout << (get_documentation_string_callback())(subcommand_name, default_help_verbosity,
                                                                           default_help_recursion_level,
                                                                           default_hide_help_status);
                        return parse_ret_t{{}, true};
                    } else if (in_namespace) {
                        std::cerr << '\"' << current_subcommand_name_string
                                  << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \""
                                  << arg_string << "\" will therefore be ignored\n";
                        invalid_input_to_namespace = true;
                        continue;
                    } else {
                        std::cerr << command_or_subcommand << " \"" << current_subcommand_name_string
                                  << "\" does not accept a flag or option \"" << arg_string
                                  << "\". It will be ignored\n";
                    }
                }
            }
        }
    }
}