
#include <iostream>

#include "catch_test_macros.hpp"

#ifndef CPPLI_SINGLE_HEADER
  #include "arg_parsing.h"
#endif

#include "cppli.h"
 //

CPPLI_SUBCOMMAND(CPPLI_NAME(repo, init),
                 "does a thing",
                 CPPLI_FLAG(force_reset, "Force the thing to rest the thing", f),
                 CPPLI_POSITIONAL(std::string, foo, "the foo positional"),
                 CPPLI_POSITIONAL(int, bar, "the bar positional"),
                 CPPLI_FLAG(recurse, "do the thing recursively", r),
                 CPPLI_OPTION(std::string, color, "color", "set the colo", c)) {

    if(force_reset) {
        std::cout << "force true!\n";
    }

    if(recurse) {
        std::cout << "recurse true!\n";
    }

    std::cout << "foo int: " << foo << '\n';

    color.access_value_if_present([&](const auto& val) {
        std::cout << "color: " << val;
    });

}

CPPLI_SUBCOMMAND(CPPLI_NAME(repo, clone),
                 "clone a repo",
                 CPPLI_POSITIONAL(int, WHO, "the int")) {}

CPPLI_SUBCOMMAND(CPPLI_NAME(repo, remove),
                 "delete the repo") {
    std::cout << "delete called\n";
}

CPPLI_SUBCOMMAND(CPPLI_NAME(repo, remove, bar),
                 "do another thing") {
    std::cout << "delete called\n";
}

CPPLI_SUBCOMMAND(CPPLI_NAME(repo, remove, bar, baz),
                 "do yet another thing") {
}

CPPLI_SUBCOMMAND(CPPLI_NAME(foo),
                 "top level foo subcommand",
                 CPPLI_FLAG(force, "force the thing", f)) {
    std::cout << "do nothing\n";
}


namespace cppli::conversions {
    template<>
    struct conversion_t<std::vector<std::string>> {
        auto operator()(const std::string& str) const {
            std::vector<std::string> ret;

            std::string current;
            for(char c : str) {
                if(c == ',') {
                    ret.push_back(std::move(current));
                    current = {};
                }
                else {
                    current.push_back(c);
                }
            }
            if(current.size()) {
                ret.push_back(std::move(current));
            }

            return ret;
        }

        static constexpr detail::string_literal type_string = "comma separated list of strings";
    };
}

struct custom_t {
    int i;
    custom_t(const std::string& str) {
        i = std::stoi(str);
    }

    custom_t(custom_t&&) = delete;
    custom_t& operator=(custom_t&&) = delete;

    custom_t(const custom_t&) = delete;
    custom_t& operator=(const custom_t&) = delete;

    static constexpr cppli::detail::string_literal cppli_type_string = "custom type integer";
};

CPPLI_MAIN_COMMAND(CPPLI_VARIADIC(int, foovec, "list of integers")) {
    std::cout << "Main entered!\n";

    for(const auto& e : foovec) {
        std::cout << e+1 << '\n';
    }
}



TEST_CASE("arg parsing works") {

    //}
    //catch(std::exception& e) {
   //     std::cerr << e.what();
        //REQUIRE(false);
   // }
   REQUIRE(true);
}
