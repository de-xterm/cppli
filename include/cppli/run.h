#pragma once

#include "detail/documentation.h"

namespace cppli {
    namespace detail {
        int run_impl_(const std::vector<std::string>& arg_vec);
        int run_impl_(int argc, const char* const* argv);

        template<string_literal program_name, string_literal description> // TODO: put run in its own file
        void set_program_name_and_description_from_template_args() {
            static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

            detail::set_program_name_and_description(program_name.string(), description.string());

            detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
            detail::subcommand_name_to_docs()[{"MAIN"}].description = description;
        }
    }

    template<string_literal program_name, string_literal description>
    int run(const std::vector<std::string>& arg_vec) {
        detail::set_program_name_and_description_from_template_args<program_name, description>();

        return detail::run_impl_(arg_vec);
    }

    template<string_literal program_name, string_literal description>
    int run(int argc, const char* const* argv) {
        detail::set_program_name_and_description_from_template_args<program_name, description>();

        return detail::run_impl_(argc, argv);
    }


    #ifdef _WIN32
        std::vector<std::string> wmain_utf16_argv_to_utf8(int argc, wchar_t** argv);

        #define cPPLI_internal_CONVERT_UTF16_IF_WINDOWS(ARGC, ARGV) cppli::wmain_utf16_argv_to_utf8(ARGC, ARGV)
        #define cPPLI_internal_MAIN_FUNCTION_HEADER int wmain(int argc, wchar_t *argv[])
    #else
        std::vector<std::string> argv_to_arg_vec(int argc, char** argv);

        #define cPPLI_internal_CONVERT_UTF16_IF_WINDOWS(ARGC, ARGV) argc_argv_to_string_vec(ARGC, ARGV)
        #define cPPLI_internal_MAIN_FUNCTION_HEADER int main(int argc, char** argv)
    #endif
}

#define CPPLI_DEFINE_MAIN_FUNCTION(PROGRAM_NAME, PROGRAM_DESCRIPTION)                                                  \
    cPPLI_internal_MAIN_FUNCTION_HEADER {                                                                              \
            return cppli::run<PROGRAM_NAME, PROGRAM_DESCRIPTION>(cPPLI_internal_CONVERT_UTF16_IF_WINDOWS(argc, argv)); \
        }

