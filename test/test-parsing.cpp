
#include "catch_test_macros.hpp"

#include "arg_parsing.h"
#include "cppli.h"


CPPLI_SUBCOMMAND(CPPLI_NAME(repo, init),
                 "does a thing",
                 CPPLI_FLAG(force_reset, "Force the thing to rest the thing", f),
                 CPPLI_POSITIONAL(cppli::int_t, foo, "the foo positional"),
                 CPPLI_FLAG(recurse, "do the thing recursively", r),
                 CPPLI_OPTION(cppli::string_t, color, "color", "set the color", c),
                 CPPLI_OPTION(cppli::string_t, Color, "color", "set the color", c)) {
    if(force_reset) {

    }

    if(foo == 30) {

    }

    if(color.has_value()) {

    }

}



TEST_CASE("arg parsing works") {
   REQUIRE(true);
}