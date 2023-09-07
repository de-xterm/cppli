
#include "catch_test_macros.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #include "cppli.h"
#else
    #include "cppli/run.h"
    #include "cppli/command_macros.h"
#endif

#include "test_utils.h"

using namespace cppli::detail;

namespace cppli {
    template<>
    struct string_conversion_t<std::vector<int>> {
        // parse comma separated list of ints
        std::vector<int> operator()(const std::string& str) const {
            std::vector<int> ret;

            std::string current_int_string;
            for(char c : str) {
                if(c == ',') {
                    ret.emplace_back(std::stoi(current_int_string));
                    current_int_string = {};
                }
                else {
                    current_int_string += c;
                }
            }

            if(current_int_string.length()) {
                ret.emplace_back(std::stoi(current_int_string));
            }

            return ret;
        }

        static constexpr cppli::string_literal type_string = "comma separated list of ints";
    };
}

struct deleted_special_member_functions {
    std::string str;
    deleted_special_member_functions(const std::string& str_) : str(str_) {}

    deleted_special_member_functions() = delete;

    deleted_special_member_functions(const deleted_special_member_functions&) = delete;
    deleted_special_member_functions& operator=(const deleted_special_member_functions&) = delete;

    deleted_special_member_functions(deleted_special_member_functions&&) = delete;
    deleted_special_member_functions& operator=(deleted_special_member_functions&&) = delete;

    static constexpr cppli::string_literal cppli_type_string = "string";
};

static std::vector<int> vec_positional;

CPPLI_SUBCOMMAND(conversions,
                 "subcommand for testing user defined conversion",
                 CPPLI_POSITIONAL(std::vector<int>, vec, "int vec positional")) {

    vec_positional = vec;
}

TEST_CASE("user defined conversion with automatically determined string_conversion_t works") {
    const char* argv[] = {"program", "conversions", "16,32,64"};
    vec_positional = {};

    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(vec_positional == std::vector{16,32,64});
}

struct semicolon_int_vec_conversion_t {
    // parse semicolon separated list of ints
    std::vector<int> operator()(const std::string& str) const {
        std::vector<int> ret;

        std::string current_int_string;
        for(char c : str) {
            if(c == ';') {
                ret.emplace_back(std::stoi(current_int_string));
                current_int_string = {};
            }
            else {
                current_int_string += c;
            }
        }

        if(current_int_string.length()) {
            ret.emplace_back(std::stoi(current_int_string));
        }

        return ret;
    }

    static constexpr cppli::string_literal type_string = "semicolon separated list of ints";
};

static std::vector<int> vec_option;

CPPLI_SUBCOMMAND(conversions2,
                 "subcommand for testing manually specified user defined_conversions",
                 CPPLI_POSITIONAL_CONVERSION(std::vector<int>, semicolon_int_vec_conversion_t, vec, "int vec positional"),
                 CPPLI_OPTION_CONVERSION_DEFAULT_CTOR(std::vector<int>, semicolon_int_vec_conversion_t, vecopt, "vector", "int vec option")) {

    vec_positional = vec;
    vec_option = vecopt;
}

static bool conversions3_called = false;
CPPLI_SUBCOMMAND(conversions3,
                 "subcommand for testing type with constructor that takes a std::string",
                 CPPLI_OPTIONAL_POSITIONAL(deleted_special_member_functions, foo, "deleted special member functions class")) {
    conversions3_called = true;
    REQUIRE(foo);
    REQUIRE(foo->str == "bar");
}

TEST_CASE("user defined conversion with manually specified string_conversion_t works") {
    const char* argv[] = {"program", "conversions2", "16;32;64"};
    vec_positional = {};

    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(vec_positional == std::vector{16,32,64});

    SECTION("works with DEFAULT_CTOR modifier") {
        const char* argv[] = {"program", "conversions2", "16;32;64", "--vecopt=16;32;64"};
        vec_option = {};

        cppli::run<"program", "does stuff">(lengthof(argv), argv);

        REQUIRE(vec_option == std::vector{16,32,64});

        SECTION("not providing option gives a default constructed object (in this case, an empty vector)") {
            const char* argv[] = {"program", "conversions2", "16;32;64"};
            vec_option = {};

            cppli::run<"program", "does stuff">(lengthof(argv), argv);

            REQUIRE(vec_option == std::vector<int>{});
        }
    }
}

TEST_CASE("class with constructor that takes std::string works") {
    const char* argv[] = {"program", "conversions3", "bar"};
    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(conversions3_called);
}
