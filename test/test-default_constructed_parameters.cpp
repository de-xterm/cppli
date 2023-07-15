
#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli::detail;

struct type_with_empty_state_t {
    bool empty = true;
    int i;

    type_with_empty_state_t(const std::string& str) : i(std::stoi(str)), empty(false) {}
    type_with_empty_state_t(int i) : i(i), empty(false) {}
    type_with_empty_state_t() = default;

    bool operator==(const type_with_empty_state_t&) const = default;

    static constexpr cppli::detail::string_literal cppli_type_string = "integer";
};

static type_with_empty_state_t empty_option;

CPPLI_SUBCOMMAND(defaultctor,
                 "subcommand for testing default constructed parameters",
                 CPPLI_OPTION_DEFAULT_CTOR(type_with_empty_state_t, empty, "number", "an option")) {

    empty_option = empty;
}

TEST_CASE("default constructed options work") {
    SECTION("they get the correct value when the option is provided") {
        const char* argv[] = {"program", "defaultctor", "--empty=36"};
        empty_option = {};

        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(empty_option == type_with_empty_state_t(36));
    }

    SECTION("they get default constructed when the option is omitted") {
        const char* argv[] = {"program", "defaultctor"};
        empty_option = {36};

        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(empty_option == type_with_empty_state_t{});
    }
}