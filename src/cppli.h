#pragma once

#include "subcommand_macros.h"
#include "subcommand.h"

namespace cppli::detail {

#define CPPLI_SUBCOMMAND(name, /*parameters*/...) \
extern "C" void cppli_internal_CAT(CPPLI_GENERATED, name) (__VA_ARGS__); \
cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_subcommand<cppli_internal_CAT(CPPLI_GENERATED, name)>({{cppli_internal_FOR_EACH(cppli_internal_STRINGIFY, name)}})) \
extern "C" void cppli_internal_CAT(CPPLI_GENERATED, name) (__VA_ARGS__)

#define CPPLI_NAME(...) __VA_ARGS__

/// the optional last argument is a single character short name
#define CPPLI_FLAG(NAME, DOCUMENTATION, /*SHORT_NAME*/...) \
const cppli::detail::flag<cppli_internal_STRINGIFY(NAME), DOCUMENTATION __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

/// the optional last argument is a single character short name
#define CPPLI_OPTION(TYPE, NAME, ARGUMENT_TEXT, DOCUMENTATION, /*SHORT_NAME*/...)

/// the optional last argument is a single character short name
#define CPPLI_REQUIRED_OPTION(TYPE, NAME, ARGUMENT_TEXT, DOCUMENTATION, /*SHORT_NAME*/...)

/// the optional last argument is a single character short name
#define CPPLI_OPTIONAL_ARGUMENT_OPTION(TYPE, NAME, ARGUMENT_TEXT, DOCUMENTATION, /*SHORT_NAME*/...)

#define CPPLI_POSITIONAL(TYPE, NAME, DOCUMENTATION)

}