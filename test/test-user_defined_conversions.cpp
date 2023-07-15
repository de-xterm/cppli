
#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli::detail;

namespace cppli::conversions {

    template<>
    struct conversion_t<std::vector<int>> {
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

        static constexpr cppli::detail::string_literal type_string = "comma separated list of ints";
    };
}

static std::vector<int> vec_positional;

CPPLI_SUBCOMMAND(conversions,
                 "subcommand for testing user defined conversion",
                 CPPLI_POSITIONAL(std::vector<int>, vec, "int vec positional")) {

    vec_positional = vec;
}

TEST_CASE("user defined conversion with automatically determined conversion_t works") {
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

    static constexpr cppli::detail::string_literal type_string = "semicolon separated list of ints";
};

CPPLI_SUBCOMMAND(conversions2,
                 "subcommand for testing manually specified user defined_conversions",
                 CPPLI_POSITIONAL_CONVERSION(std::vector<int>, semicolon_int_vec_conversion_t, vec, "int vec positional")) {

    vec_positional = vec;
}

TEST_CASE("user defined conversion with manually specified conversion_t works") {
    const char* argv[] = {"program", "conversions2", "16;32;64"};
    vec_positional = {};

    cppli::run<"program", "does stuff">(lengthof(argv), argv);

    REQUIRE(vec_positional == std::vector{16,32,64});
}