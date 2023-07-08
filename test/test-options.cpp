
#include <string>
#include <optional>

#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli::detail;

static bool foo_flag;
static bool bar_flag;
static bool baz_flag;

static std::optional<std::string> color_option;
static std::optional<int> size_option;

CPPLI_SUBCOMMAND(opttest,
                 "takes some flags and some options",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_FLAG(bar, "bar flag", b),
                 CPPLI_FLAG(baz, "baz flag", z),

                 CPPLI_OPTION(std::string, color, "color", "color option", c),
                 CPPLI_OPTION(int, size, "color", "color option", s)) {
    foo_flag = foo;
    bar_flag = bar;
    baz_flag = baz;

    if(color) {
        color_option = *color;
    }

    if(size) {
        size_option = *size;
    }
}


TEST_CASE("passing a single option works") {
    SECTION("with long name") {
        SECTION("with equals syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "--size=32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }

        SECTION("with equals syntax") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            color_option = std::nullopt;
            size_option  = std::nullopt;

            const char* argv[] = {"program", "opttest", "--size", "32"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);

            REQUIRE(!color_option);

            REQUIRE(size_option);
            REQUIRE(*size_option == 32);
        }
    }

    SECTION("with short name") {
        // TODO:
    }
}