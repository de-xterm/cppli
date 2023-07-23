#pragma once

#include "detail/subcommand_macros.h"
#include "detail/command_registration.h"

#ifdef _MSC_VER
    #ifndef _MSVC_TRADITIONAL
        #error "CPPLI won't work on this version of MSVC, sorry! The version of MSVC you're using doesn't support the new preprocessor (/Zc:preprocessor)"
    #elif _MSVC_TRADITIONAL == 1
        #error "You're using the traditional preprocessor, which won't work with CPPLI. Recompile with /Zc:preprocessor"
    #endif
#endif

namespace cppli {
    bool current_command_is_leaf();

    //#define CPPLI_NAMESPACE(NAME, DESCRIPTION)

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
    const ::cppli::detail::option<TYPE, ::cppli::conversions::conversion_t<std::optional<TYPE>>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const std::optional<TYPE>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const std::optional<TYPE>& NAME

    /// same as CPPLI_OPTION, but the raw TYPE is used (without std::optional)
    /// If the option is not provided, a default constructed object is passed to the callback instead of an empty optional
    /// The optional last argument is a single character short name
    #define CPPLI_OPTION_DEFAULT_CTOR(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversions::conversion_t<TYPE>, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const TYPE& NAME

    #define CPPLI_OPTION_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const TYPE& NAME


    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversions::conversion_t<std::optional<TYPE>>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_DEFAULT_CTOR(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversions::conversion_t<std::optional<TYPE>>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME



    /// the optional last argument is a single character short name
    #define CPPLI_REQUIRED_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversions::conversion_t<TYPE>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_REQUIRED_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME


    #define CPPLI_POSITIONAL(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::conversions::conversion_t<TYPE>, false, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

    #define CPPLI_POSITIONAL_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME


    #define CPPLI_OPTIONAL_POSITIONAL(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::conversions::conversion_t<std::optional<TYPE>>, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::optional<TYPE>& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::optional<TYPE>& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_DEFAULT_CTOR(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::conversions::conversion_t<TYPE>, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME


    #define CPPLI_VARIADIC(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::variadic<TYPE, ::cppli::conversions::conversion_t<TYPE>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME

    #define CPPLI_VARIADIC_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::variadic<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME
}