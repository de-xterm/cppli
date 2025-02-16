
#include "catch_test_macros.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #include "cppli.h"
#else
    #include "cppli/run.h"
    #include "cppli/command_macros.h"
#endif

#include "test_utils.h"

using namespace cppli::detail;
using namespace cppli;

static bool foo_flag;
static bool bar_flag;
static bool baz_flag;

// TODO: add test case for trying to give an option an argument throwing a cli_error

CPPLI_SUBCOMMAND(flagtest,
                 "takes some flags",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_FLAG(bar, "bar flag", b),
                 CPPLI_FLAG(baz, "baz flag", z)) {

    foo_flag = foo;
    bar_flag = bar;
    baz_flag = baz;
}

TEST_CASE("passing a single flag works") {
    SECTION("with long name") {
        SECTION("providing the flag evaluates to true") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            const char* argv[] = {"program", "flagtest", "--foo"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);
        }

        SECTION("omitting the flag evaluates to false") {
            foo_flag = true;
            bar_flag = true;
            baz_flag = true;

            const char* argv[] = {"program", "flagtest"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(!foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);
        }
    }

    SECTION("with short name") {
        SECTION("providing the flag evaluates to true") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            const char* argv[] = {"program", "flagtest", "-f"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(foo_flag);
            REQUIRE(!bar_flag);
            REQUIRE(!baz_flag);
        }
    }
}

TEST_CASE("passing multiple flags in separate groups works") {
    SECTION("with long name") {
        SECTION("providing the flags evaluates to true") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            const char* argv[] = {"program", "flagtest", "--foo", "--bar", "--baz"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(foo_flag);
            REQUIRE(bar_flag);
            REQUIRE(baz_flag);
        }
    }

    SECTION("with short name") {
        SECTION("providing the flags evaluates to true") {
            foo_flag = false;
            bar_flag = false;
            baz_flag = false;

            const char* argv[] = {"program", "flagtest", "-f", "-b", "-z"};
            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(foo_flag);
            REQUIRE(bar_flag);
            REQUIRE(baz_flag);
        }
    }
}

TEST_CASE("passing multiple flags in the same groups works") {
    SECTION("providing the flags evaluates to true") {
        foo_flag = false;
        bar_flag = false;
        baz_flag = false;

        const char* argv[] = {"program", "flagtest", "-fbz"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(foo_flag);
        REQUIRE(bar_flag);
        REQUIRE(baz_flag);
    }
}

TEST_CASE("passing invalid characters in flag groups throws") {
    SECTION("with random numbers") {
        const char* argv[] = {"program", "flagtest", "-fbz34239"};

        REQUIRE_THREW(cppli::cli_error, INVALID_FLAG, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    SECTION("with equals and non-empty argument") {
        const char* argv[] = {"program", "flagtest", "-fbz=foo"};

        REQUIRE_THREW(cppli::cli_error, FLAG_GIVEN_AN_ARGUMENT, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));

    }

    SECTION("with equals and empty argument") {
        const char* argv[] = {"program", "flagtest", "-fbz="};

        REQUIRE_THREW(cppli::cli_error, FLAG_GIVEN_AN_ARGUMENT, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }
}

TEST_CASE("repeated flag throw error") {
    {
        const char* argv[] = {"program", "flagtest", "--foo", "--foo"};
        REQUIRE_THREW(cppli::cli_error, FLAG_INCLUDED_MULTIPLE_TIMES, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    {
        const char* argv[] = {"program", "flagtest", "--foo", "-f"};
        REQUIRE_THREW(cppli::cli_error, FLAG_INCLUDED_MULTIPLE_TIMES, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    {
        const char* argv[] = {"program", "flagtest", "-f", "-f"};
        REQUIRE_THREW(cppli::cli_error, FLAG_INCLUDED_MULTIPLE_TIMES, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    {
        const char* argv[] = {"program", "flagtest", "-ff"};
        REQUIRE_THREW(cppli::cli_error, FLAG_INCLUDED_MULTIPLE_TIMES, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }
}