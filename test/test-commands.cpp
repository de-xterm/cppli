
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

static bool main_command_called = false;

static bool main_is_leaf;
static bool foo_is_leaf;
static bool bar_is_leaf;

CPPLI_MAIN_COMMAND() {
    main_command_called = true;

    main_is_leaf = cppli::current_command_is_leaf();
}

static bool foo_subcommand_called = false;
CPPLI_SUBCOMMAND(foo,
                 "the foo subcommand") {

    foo_subcommand_called = true;
    foo_is_leaf = cppli::current_command_is_leaf();
}

static bool bar_subcommand_called = false;
CPPLI_SUBCOMMAND(CPPLI_NAME(foo, bar),
                 "the bar subcommand",

                 CPPLI_OPTIONAL_POSITIONAL(int, size, "size positional")) {

    bar_subcommand_called = true;
    bar_is_leaf = cppli::current_command_is_leaf();
}

static bool foo_in_namespace_called = false;
CPPLI_SUBCOMMAND(CPPLI_NAME(namespace, foo),
                 "namespace::foo subcommand") {
    foo_in_namespace_called = true;
}

static bool help_subcommand_called = false;
static bool help_is_leaf = false;

CPPLI_SUBCOMMAND(CPPLI_NAME(foo, help),
                 "custom help subcommand",

                 CPPLI_OPTIONAL_POSITIONAL(int, size, "size positional")) {

    help_subcommand_called = true;
    help_is_leaf = cppli::current_command_is_leaf();
}


TEST_CASE("main command callback gets called") {
    const char* argv[] = {"program"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(main_command_called);
    REQUIRE(main_is_leaf);
}

TEST_CASE("subcommand callback gets called") {
    const char* argv[] = {"program", "foo"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(foo_subcommand_called);

    REQUIRE(!main_is_leaf);
    REQUIRE(foo_is_leaf);
}

TEST_CASE("nested subcommand callback gets called") {
    const char* argv[] = {"program", "foo", "bar"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(!main_is_leaf);
    REQUIRE(!foo_is_leaf);
    REQUIRE(bar_is_leaf);

    REQUIRE(bar_subcommand_called);
}

TEST_CASE("misspelling subcommand name throws") {
    SECTION("at a branch subcommand") {
        const char* argv[] = {"program", "fo", "bar"};
        REQUIRE_THREW(cppli::user_error, EXCESS_POSITIONAL, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));

        user_error::error_variant_t  v;
    }

    SECTION("at a leaf subcommand") {
        const char* argv[] = {"program", "foo", "ba"};
        REQUIRE_THREW(cppli::user_error, EXCESS_POSITIONAL, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
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

    REQUIRE_THREW(cppli::user_error, STRING_CONVERSION_ERROR, (cppli::run<"program", "does stuff">(lengthof(argv), argv)));
    REQUIRE(!foo_subcommand_called);
    REQUIRE(!bar_subcommand_called);
}

TEST_CASE("custom help subcommand gets called") {
    help_subcommand_called = false;
    help_is_leaf = false;

    const char* argv[] = {"program", "foo", "help"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(help_is_leaf);
    REQUIRE(help_subcommand_called);
}