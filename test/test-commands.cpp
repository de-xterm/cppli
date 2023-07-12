
#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli::detail;

static bool main_command_called = false;

CPPLI_MAIN_COMMAND() {
    main_command_called = true;
}

static bool foo_subcommand_called = false;
CPPLI_SUBCOMMAND(foo,
                 "the foo subcommand") {
    foo_subcommand_called = true;
}

static bool bar_subcommand_called = false;
CPPLI_SUBCOMMAND(CPPLI_NAME(foo, bar),
                 "the bar subcommand",

                 CPPLI_OPTIONAL_POSITIONAL(int, size, "size positional")) {
    bar_subcommand_called = true;
}

static bool foo_in_namespace_called = false;
CPPLI_SUBCOMMAND(CPPLI_NAME(namespace, foo),
                 "namespace::foo subcommand") {
    foo_in_namespace_called = true;
}


TEST_CASE("main command callback gets called") {
    const char* argv[] = {"program"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(main_command_called);
}

TEST_CASE("subcommand callback gets called") {
    const char* argv[] = {"program", "foo"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(foo_subcommand_called);
}

TEST_CASE("nested subcommand callback gets called") {
    const char* argv[] = {"program", "foo", "bar"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(bar_subcommand_called);
}

TEST_CASE("misspelling subcommand name throws") {
    SECTION("at a branch subcommand") {
        const char* argv[] = {"program", "fo", "bar"};
        REQUIRE_THREW(cppli::user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }

    SECTION("at a leaf subcommand") {
        const char* argv[] = {"program", "foo", "ba"};
        REQUIRE_THREW(cppli::user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    }
}

TEST_CASE("attempting to use a namespace without further subcommands does nothing") {
    const char* argv[] = {"program", "namespace"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(!foo_in_namespace_called);
}

TEST_CASE("Branch subcommands are not run if a conversion error occurs at a leaf subcommand") {
    const char* argv[] = {"program", "foo", "bar", "not-an-int"};

    foo_subcommand_called = false;
    bar_subcommand_called = false;

    REQUIRE_THREW(cppli::user_error, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    REQUIRE(!foo_subcommand_called);
    REQUIRE(!bar_subcommand_called);
}