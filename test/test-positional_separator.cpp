
#include <string>

#include "catch_test_macros.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #include "cppli.h"
#else
    #include "cppli/run.h"
    #include "cppli/command_macros.h"
#endif

#include "test_utils.h"

using namespace cppli::detail;

static std::vector<std::string> var_variadic;
static bool foo_flag;
static bool bar_flag;

CPPLI_SUBCOMMAND(posseparatortest,
                 "subcommand for testing the positional separator",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_FLAG(bar, "bar flag", b),
                 CPPLI_VARIADIC(std::string, var, "positional argument")) {
    
    var_variadic = var;
    foo_flag = foo;
    bar_flag = bar;
}


static bool subcommand_called = false;
static std::optional<std::string> subcommand_pos;
CPPLI_SUBCOMMAND(CPPLI_NAME(posseparatortest, subcommand),
                 "subcommand for testing the positional separator",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_OPTIONAL_POSITIONAL(std::string, pos, "positional argument")) {

    subcommand_called = true;
    subcommand_pos = pos;
}

TEST_CASE("--1 separator works") {
    SECTION("for short flags") {
        var_variadic = {};

        const char* argv[] = {"program", "posseparatortest", "--1", "-f", "-b"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(var_variadic == std::vector<std::string>{"-f"});

        REQUIRE(bar_flag);
        REQUIRE(!foo_flag);
    }

    SECTION("for long flags") {
        var_variadic = {};

        const char* argv[] = {"program", "posseparatortest", "--1", "--foo", "-b"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(var_variadic == std::vector<std::string>{"--foo"});

        REQUIRE(bar_flag);
        REQUIRE(!foo_flag);
    }

    SECTION("for subcommand matches") {
        var_variadic = {};

        const char* argv[] = {"program", "posseparatortest", "--1", "subcommand"};
        subcommand_called = false;
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(var_variadic == std::vector<std::string>{"subcommand"});

        REQUIRE(!subcommand_called);
    }
}

TEST_CASE("-- separator works") {
    SECTION("for flags") {
        var_variadic = {};

        const char* argv[] = {"program", "posseparatortest", "--", "-f", "--bar"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(var_variadic == std::vector<std::string>{"-f", "--bar"});
    }

    SECTION("doesn't disambiguate subcommand matches") {
        var_variadic = {};
        subcommand_pos = std::nullopt;

        const char* argv[] = {"program", "posseparatortest", "--", "subcommand", "string"};
        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(var_variadic.size() == 0);

        REQUIRE(subcommand_called);
        REQUIRE(subcommand_pos);
        REQUIRE(*subcommand_pos == "string");
    }
}

TEST_CASE("--- separator works") {
    var_variadic = {};

    const char* argv[] = {"program", "posseparatortest", "---", "subcommand", "--bar", "-f"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(var_variadic == std::vector<std::string>{"subcommand", "--bar", "-f"});
    REQUIRE(!bar_flag);
    REQUIRE(!bar_flag);
}