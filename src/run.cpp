
#define UTF_CPP_CPLUSPLUS 202002L // we have to do this because MSVC's __cplusplus is broken and the developer of utfcpp refuses to use _MSC_LANG because he wants to "avoid any compiler-specific code" (???)
                                  // I'm not sure what his reasoning is for that policy
                                  // https://github.com/nemtrif/utfcpp/pull/125
#include "utf8.h"

#include "iro.h"

#include "arg_parsing.h"
#include "documentation.h"
#include "command_registration.h"


namespace cppli {
    namespace detail {
        extern command_name_t last_subcommand_;

        int run_impl_(const std::vector<std::string>& arg_vec) {
            try {
                {
                    command_name_t main_help = {"MAIN", "help"};
                    if (!subcommand_name_to_func().contains(main_help)) {
                        register_command<default_help_callback>(main_help, "print help for this command", true);
                    }
                }

                auto parse_ret = detail::parse(arg_vec);

                if (parse_ret.help_command_index && !parse_ret.printed_help) {
                    // help command is special. Skip all other execution if it is encountered
                    const auto& help_command = parse_ret.subcommands[*parse_ret.help_command_index];
                    last_subcommand_ = parse_ret.subcommands[parse_ret.subcommands.size() - 2].name;
                    (detail::subcommand_name_to_func()[help_command.name])(help_command, {true});
                }                                                                         // help is always leaf
                else if (!parse_ret.printed_help) {
                    const auto& commands_vec = parse_ret.subcommands;

                    #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
                        for(const auto& command : commands_vec) { // throws if any errors would occur calling the given commands, without actually calling them
                            if(detail::subcommand_name_to_error_checking_func().contains(command.name)) {
                                (detail::subcommand_name_to_error_checking_func()[command.name])(command);
                            }
                        }
                    #endif


                    bool runnable_command_found = false;

                    if (!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                        (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0], {(commands_vec.size() < 2)});
                        runnable_command_found = true;
                        detail::last_subcommand_ = {};
                    }

                    for (unsigned i = 1; i < commands_vec.size(); ++i) {
                        detail::last_subcommand_ = commands_vec[i - 1].name;
                        if ((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                            runnable_command_found = true;
                            (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i], {(i == commands_vec.size() - 1)});
                        }
                    }

                    if (!runnable_command_found) {
                        throw cli_error("The input did not form any runnable commands", NO_RUNNABLE_COMMAND_FOUND);
                    }
                }
            }
            catch(cppli::application_error& e) {
                print_fancy_error(std::cerr, e.what());
                return 1; /// by convention, exit code 1 is for application errors https://stackoverflow.com/a/40484670
            }
            catch(cppli::cli_error& e) {
                print_fancy_error(std::cerr, e.what());
                return 2; /// by convention, exit code 2 is for command-line errors https://stackoverflow.com/a/40484670
            }
        }
    }

    #ifdef _WIN32
        std::vector<std::string> wmain_utf16_argv_to_utf8(int argc, wchar_t** argv) {
            static_assert(sizeof(wchar_t) == sizeof(char16_t), "paranoia size check. This assert should never fail");
            static_assert(sizeof(wchar_t) * CHAR_BIT == 16,    "paranoia size check. This assert should never fail");

            if(argc == 0) {
                std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argc == 0. This is very terrible and unrecoverable\n";
                std::exit(-1);
            }
            if(!argv[0]) {
                std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argv[0] was null. This is very terrible and unrecoverable\n";
                std::exit(-1);
            }

            std::vector<std::string> ret;
            ret.resize(argc);
            for (unsigned i = 0; i < argc; ++i) {
                /// two memcpy calls to convert utf16 to utf8 IN ADDITION to using functions from a utility library. Absolute state of C++
                /// if there is a better way to do this, please let me know (if std::start_lifetime_as ever gets implemented, it could be used to reduce the number of copies)
                auto wstr = std::wstring_view(argv[i]);

                std::u16string u16string;
                u16string.resize(wstr.size());
                std::memcpy(u16string.data(), wstr.data(), sizeof(wchar_t) * wstr.size());

                std::u8string u8string = utf8::utf16tou8(u16string);
                ret[i].resize(u8string.size());
                std::memcpy(ret[i].data(), u8string.data(), sizeof(char8_t) * u8string.size());
            }

            return ret; //nrvo
        }
    #else
        std::vector<std::string> argv_to_arg_vec(int argc, const char* const* argv) {
            if(argc == 0) {
                std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argc == 0. This is very terrible and unrecoverable\n";
                std::exit(-1);
            }
            if(!argv[0]) {
                std::cerr << iro::bright_red << iro::effect_string(iro::bold|iro::underlined, "Error:") << " argv[0] was null. This is very terrible and unrecoverable\n";
                std::exit(-1);
            }

            std::vector<std::string> arg_vec(argc);
            for(unsigned i = 0; i < argc; ++i) {
                arg_vec[i] = argv[i];
            }
            return arg_vec;
        }
    #endif
}

