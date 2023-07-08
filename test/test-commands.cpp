
#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli::detail;

bool main_command_called = false;

CPPLI_MAIN_COMMAND() {
    main_command_called = true;
}

bool foo_subcommand_called = false;
CPPLI_SUBCOMMAND(foo,
                 "the foo subcommand") {
    foo_subcommand_called = true;
}

bool bar_subcommand_called = false;
CPPLI_SUBCOMMAND(CPPLI_NAME(foo, bar),
                 "the bar subcommand") {
    bar_subcommand_called = true;
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