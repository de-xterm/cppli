
#include <stdexcept>
#include <cassert>
#include <iostream>

#include "arg_parsing.h"

namespace cppli::detail {
    enum parse_mode {
        COMMAND,
        ARG
    };

    std::vector<subcommand_args_t> parse(int argc, char** argv) {
                          // skip the program name
        std::vector<subcommand_args_t> commands;

        subcommand_name_t subcommand_name;
        parse_mode mode;

        subcommand_args_t args;
        for(unsigned arg_i = 1; arg_i < argc; ++arg_i) { // TODO: message ignoring unrecognized options/flags/args
            std::string arg_string = argv[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string)) {
                commands.push_back(std::move(args));
                args = {};
            }
            else {
                if(arg_string.substr(0,2) == "--") { // long flag (these are ez)
                    std::string::size_type equals_pos;
                    if((equals_pos = arg_string.find('=')) != std::string::npos) {
                        std::string option_name  = arg_string.substr(2, equals_pos),
                                    option_value = arg_string.substr(equals_pos+1, arg_string.size()-(equals_pos+1));

                        if(subcommand_takes_option(subcommand_name, option_name)) {
                            args.options_to_values.emplace(option_name, option_value);
                        }
                        else if(subcommand_takes_flag(subcommand_name, option_name)) {
                            std::cerr << "For (sub)command \"" << ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) << "\", \"" << option_name << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \"" << arg_string << "\"). "
                                         "The value will be ignored and the flag will be set to true\n";

                            args.flags.emplace(option_name);
                        }
                        else {
                            std::cerr << "Flag/option \"" << option_name << "\" (from \"" << arg_string << "\") was not recognized by (sub)command \"" << ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) << "\" and is therefore being ignored\n";
                        }
                    }
                    else {
                        std::string option_or_flag_name = arg_string.substr(2, arg_string.size()-1);

                        if(subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                            if(subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                                args.options_to_values.emplace(option_or_flag_name, "");
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

                        }
                        else {
                            throw std::runtime_error("(Sub)command \"" + ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) + "\" does not accept a flag or option \"" + arg_string + '\"');
                        }
                    }
                }
                else if(arg_string[0] == '-') { // short flag(s) and/or option (these are not ez)
                    if(arg_string.find('=') != std::string::npos) {
                        for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                            std::string char_string = arg_string.substr(char_i,1);
                            if((char_i < arg_string.size()-1) && (arg_string[char_i+1] == '=')) {
                                if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, arg_string.substr(char_i+2, arg_string.size()-(char_i+2)));
                                }
                                else if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    std::cerr << "For (sub)command \"" << ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) << "\", \"" << char_string << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in flag/option group \"" << arg_string << "\"). "
                                                 "The value will be ignored and the flag will be set to true\n";

                                    args.flags.emplace(char_string);
                                }
                                else {
                                    std::cerr << "Flag/option \"" << char_string << "\" (from flag/option group \"" << arg_string << "\") was not recognized by (sub)command \"" << ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) << "\" and is therefore being ignored\n";
                                }
                                break;
                            }
                            else {
                                if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    args.flags.emplace(char_string);
                                }
                                else if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, "");
                                }
                                else {
                                    std::cerr << "Flag/option \"" << char_string << "\" (from flag/option group \"" << arg_string << "\") was not recognized by (sub)command \"" << ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) << "\" and is therefore being ignored\n";
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
                                }
                                else if(char_i == arg_string.size()-1) {
                                    if(subcommand_option_argument_is_optional(subcommand_name, char_string)) { // if this option's argument is optional, assume that no argument is provided, and that the next arg is unrelated
                                        args.options_to_values.emplace(char_string, "");
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
                                    assert(false);
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, char_string)) {
                                args.flags.emplace(char_string);
                            }
                            else {
                                throw std::runtime_error("Character '" + char_string + "' in flag/option group \"" + arg_string + "\" "
                                                         "did not form a valid flag or option for (sub)command \"" + ((subcommand_name.size() == 0) ? argv[0] : subcommand_name.back()) + '\"');
                            }
                        }
                    }
                }
                else { // positional arg
                    if(arg_string[0] == '\\') {
                        args.positional_args.emplace_back(std::move(arg_string).substr(1));
                    }
                    else {
                        args.positional_args.emplace_back(std::move(arg_string));
                    }
                }
            }
        }
    }
}