#pragma once

#include "detail/documentation.h"

namespace cppli {

    namespace detail {
        void run_impl_(const std::vector<std::string>& arg_vec);
        void run_impl_(int argc, const char* const* argv);

        template<string_literal program_name, string_literal description> // TODO: put run in its own file

        void set_program_name_and_description_() {
            static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

            detail::set_program_name_and_description(program_name.string(), description.string());

            detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
            detail::subcommand_name_to_docs()[{"MAIN"}].description = description;
        }
    }

    template<string_literal program_name, string_literal description> // TODO: put run in its own file
    void run(const std::vector<std::string>& arg_vec) {
        set_program_name_and_description_<program_name, description>();

        detail::run_impl_(arg_vec);
    }

    template<string_literal program_name, string_literal description> // TODO: put run in its own file
    void run(int argc, const char* const* argv) {
        set_program_name_and_description_<program_name, description>();

        detail::run_impl_(argc, argv);
    }

    #ifdef _WIN32
        std::vector<std::string> wmain_utf16_argv_to_utf8(int argc, wchar_t** argv);
    #endif
}


#ifdef _WIN32
    #define CPPLI_MAIN_FUNCTION(PROGRAM_NAME, PROGRAM_DESCRIPTION)                                                                      \
        int int wmain(int argc, wchar_t *argv[]) {                                                                                      \
            try {                                                                                                                       \
                run<PROGRAM_NAME, PROGRAM_DESCRIPTION>(wmain_utf16_argv_to_utf8(argc, argv));                                           \
            }                                                                                                                           \
            catch(cppli::user_error& e) {                                                                                               \
                std::cerr << e.what() << '\n';                                                                                          \
                return -1;                                                                                                              \
            }                                                                                                                           \
            /*TODO: I really need to add support for return codes and have run() return one. Using std::exit() for everything is lame*/ \
            return 0;                                                                                                                   \
        }
#else
    #define CPPLI_MAIN_FUNCTION(PROGRAM_NAME, PROGRAM_DESCRIPTION)                                                                      \
        int int wmain(int argc, wchar_t *argv[]) {                                                                                      \
            try {                                                                                                                       \
                run<PROGRAM_NAME, PROGRAM_DESCRIPTION>(argc, argv);                                                                     \
            }                                                                                                                           \
            catch(cppli::user_error& e) {                                                                                               \
                std::cerr << e.what() << '\n';                                                                                          \
                return -1;                                                                                                              \
            }                                                                                                                           \
            /*TODO: I really need to add support for return codes and have run() return one. Using std::exit() for everything is lame*/ \
            return 0;                                                                                                                   \
        }
#endif

