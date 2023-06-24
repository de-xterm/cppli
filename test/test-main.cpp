#include "catch_session.hpp"

#include "cppli.h"

int main( int argc, char* argv[] ) {
    // your setup ...

    //int argc = 5;
    // try {
    std::cout << cppli::get_documentation_string({"repo"}, cppli::NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS, 2);
    cppli::run<"program", "it does a thing">(argc, argv);

    //int result = Catch::Session().run( argc, argv );

    // your clean-up...

    //return result;
}