
#include <string>
#include <optional>

#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli;
using namespace cppli::detail;

static std::optional<int> size_option;

int required_positional;
std::optional<int> optional_positional;
std::vector<int> variadic_positional;

CPPLI_SUBCOMMAND(postest,
                 "takes some flags and some options",
                 CPPLI_POSITIONAL(int, required, "required positional"),
                 CPPLI_OPTIONAL_POSITIONAL(int, optional, "optional positional"),
                 CPPLI_VARIADIC(int, variadic, "variadic positional")) {
    
    required_positional = required;
    optional_positional = optional;
    variadic_positional = variadic;
}

TEST_CASE("required positionals are assigned correctly") {
    required_positional = 0;

    const char* argv[] = {"program", "postest", "32"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);
    REQUIRE(required_positional == 32);

    SECTION("not including a required positional throws") {
        const char* argv[] = {"program", "postest"};
        REQUIRE_THREW(user_error, REQUIRED_POSITIONAL_NOT_PROVIDED, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }
}

TEST_CASE("optional positionals work and don't mess with required positionals") {
    required_positional = 0;
    optional_positional = 0;

    const char* argv[] = {"program", "postest", "32", "64"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(required_positional == 32);
    REQUIRE(optional_positional);
    REQUIRE(*optional_positional == 64);
}

TEST_CASE("variadic positionals work and don't mess with required positionals or optional positionals") {
    required_positional = 0;
    optional_positional = 0;
    variadic_positional = {};

    const char* argv[] = {"program", "postest", "32", "64", "128", "256", "512"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(required_positional == 32);
    REQUIRE(optional_positional);
    REQUIRE(*optional_positional == 64);
    REQUIRE(variadic_positional == std::vector{128, 256, 512});
}