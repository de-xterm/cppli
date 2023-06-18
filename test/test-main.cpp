#include "catch_session.hpp"

#include "cppli.h"

int main( int argc, char* argv[] ) {
    // your setup ...

    //int argc = 5;
    //const char* argv[] = {"app", "repo", "fard", "30", "--color=blue"};
    // try {
    cppli::run(argc, argv);

    //int result = Catch::Session().run( argc, argv );

    // your clean-up...

    //return result;
}