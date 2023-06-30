#pragma once

#include "subcommand_macros.h"
#include "arg_parsing.h"
#include "command_registration.h"

namespace cppli {
    template<detail::string_literal program_name, detail::string_literal description>
    void run(int argc, const char* const* const argv) {
        static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

        detail::set_program_name_and_description(program_name.string(), description.string());

        detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
        detail::subcommand_name_to_docs()[{"MAIN"}].description = description;

        auto parse_ret = detail::parse(argc, argv);

        if(!parse_ret.printed_help) {
            const auto& commands_vec = parse_ret.subcommands;

            bool runnable_command_found = false;
            if(!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0]);
                runnable_command_found = true;
            }

            for(unsigned i = 1; i < commands_vec.size(); ++i) {
                if((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                    runnable_command_found = true;
                    (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i]);
                }
            }

            if(!runnable_command_found) {
                std::cerr << "The input did not form any runnable commands\n";
                // TODO: print help here?
            }
        }
    }

    namespace detail {
        #define CPPLI_NAMESPACE(NAME, DESCRIPTION)

        #define CPPLI_MAIN_COMMAND(/*parameters*/...) \
        extern "C" void CPPLI_GENERATED_MAIN (__VA_ARGS__); \
        cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_subcommand<CPPLI_GENERATED_MAIN>({"MAIN"}, "")) \
        extern "C" void CPPLI_GENERATED_MAIN (__VA_ARGS__)

        #define CPPLI_SUBCOMMAND(name, DESCRIPTION, /*parameters*/...) \
        extern "C" void cppli_internal_CAT(CPPLI_GENERATED, name) (__VA_ARGS__); \
        static_assert(!::cppli::detail::contains_uppercase<cppli_internal_STRINGIFY(cppli_internal_CAT(name))>(), "subcommand names cannot contain uppercase characters"); \
        cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_subcommand<cppli_internal_CAT(CPPLI_GENERATED, name)>({cppli_internal_FOR_EACH(cppli_internal_STRINGIFY_WITH_COMMA, MAIN, name)}, DESCRIPTION)) \
        extern "C" void cppli_internal_CAT(CPPLI_GENERATED, name) (__VA_ARGS__)

        #define CPPLI_NAME(...) __VA_ARGS__

        /// the optional last argument is a single character short name
        #define CPPLI_FLAG(NAME, DESCRIPTION, /*SHORT_NAME*/...) \
        const ::cppli::detail::flag<cppli_internal_STRINGIFY(NAME), DESCRIPTION __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, bool NAME

        /// the optional last argument is a single character short name
        #define CPPLI_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
        const ::cppli::detail::option<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

        /// the optional last argument is a single character short name
        #define CPPLI_REQUIRED_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
        const ::cppli::detail::option<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME

        /// the optional last argument is a single character short name
        #define CPPLI_OPTIONAL_ARGUMENT_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
        __VA_OPT__(static_assert(isletter(__VA_ARGS__)[0]))                                               \
        const ::cppli::detail::option<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

        #define CPPLI_POSITIONAL(TYPE, NAME, DESCRIPTION) \
        const ::cppli::detail::positional<TYPE, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

        #define CPPLI_OPTIONAL_POSITIONAL(TYPE, NAME, DESCRIPTION) \
        const ::cppli::detail::positional<TYPE, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>& NAME
    }
}