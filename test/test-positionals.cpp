
#include <string>
#include <optional>

#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli;
using namespace cppli::detail;

static bool foo_flag;

static std::optional<int> size_option;


CPPLI_SUBCOMMAND(postest,
                 "takes some flags and some options",
                 CPPLI_POSITIONAL(int, required, "required positional"),
                 CPPLI_OPTIONAL_POSITIONAL(int, optional, "optional positional"),
                 CPPLI_VARIADIC(int, var, "variadic positional")) {

}
