
#include <stdexcept>
#include <cassert>

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
        for(unsigned arg_i = 1; arg_i < argc; ++arg_i) {
            std::string arg_string = argv[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string)) {
                commands.push_back(std::move(args));
                args = {};
            }
            else {
                if(arg_string.substr(0,2) == "--") { // long flag (these are ez)

                }
                else if(arg_string[0] == '-') { // short flag(s) and/or option (these are not ez)
                    if(arg_string.find('=') != std::string::npos) {
                        if(arg_string[0] != '-') {
                            throw std::runtime_error("")
                        }
                        for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                            std::string char_string = arg_string.substr(char_i,1);
                            if((char_i < arg_string.size()-1) && (arg_string[char_i+1] == '=')) {
                                args.options_to_values.emplace(char_string, arg_string.substr(char_i+2, arg_string.size()-(char_i+2)));
                                break;
                            }
                            else {
                                if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    args.flags.emplace(char_string);
                                }
                                else if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, "");
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
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(char_string, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        throw std::runtime_error(std::string("the last character (") + arg_string[char_i] + ") in flag/option group \"" + arg_string + "\" referred to an option with a mandatory argument, "
                                                                                                                                                                       "but no argument followed\n");
                                    }
                                }
                                else {
                                    assert(false);
                                }
                                /*if((char_i != arg_string.size()-1) && (arg_string[char_i+1] != '=')) {
                                    throw std::runtime_error(std::string("Couldn't "))
                                }*/

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

                }
            }
        }
    }
}