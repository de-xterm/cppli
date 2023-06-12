#include "catch_session.hpp"

#include "arg_parsing.h"

int main( int argc, char* argv[] ) {
    // your setup ...



    cppli::detail::parse(argc, argv);
    int result = Catch::Session().run( argc, argv );

    // your clean-up...

    return result;
}