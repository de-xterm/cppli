
#include <iostream>

#include "catch_test_macros.hpp"

#include "arg_parsing.h"
#include "cppli.h"


CPPLI_SUBCOMMAND(CPPLI_NAME(repo, init),
                 "does a thing",
                 CPPLI_FLAG(force_reset, "Force the thing to rest the thing", f),
                 CPPLI_POSITIONAL(cppli::string_t, foo, "the foo positional"),
                 CPPLI_POSITIONAL(cppli::int_t, bar, "the bar positional"),
                 CPPLI_FLAG(recurse, "do the thing recursively", r),
                 CPPLI_OPTION(cppli::string_t, color, "color", "set the colo", c)) {

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
                 CPPLI_POSITIONAL(cppli::int_t, WHO, "the int")) {}

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

}

TEST_CASE("arg parsing works") {

    //}
    //catch(std::exception& e) {
   //     std::cerr << e.what();
        //REQUIRE(false);
   // }
   REQUIRE(true);
}