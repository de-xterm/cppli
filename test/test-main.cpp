#include "catch_session.hpp"

#include "cppli.h"


int main( int argc, char* argv[] ) {
    // your setup ...

    //int argc = 5;
    // try {
    cppli::run<"program", "it does a thing">(argc, argv);

    //int result = Catch::Session().run( argc, argv );

    // your clean-up...

    //return result;
}