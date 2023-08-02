#pragma once

#include "detail/documentation.h"

namespace cppli {

    namespace detail {
        void run_impl_(int argc, const char* const* const argv);
    }

    template<detail::string_literal program_name, detail::string_literal description> // TODO: put run in its own file
    void run(int argc, const char* const* const argv) {
        static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

        detail::set_program_name_and_description(program_name.string(), description.string());

        detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
        detail::subcommand_name_to_docs()[{"MAIN"}].description = description;

        detail::run_impl_(argc, argv);
    }
}