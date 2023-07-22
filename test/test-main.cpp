
#include "catch_session.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #define CPPLI_IMPLEMENTATION
#endif

#include "cppli.h"

int main(int argc, char** argv) {
    cppli::run<"program", "does stuff">(argc, argv);

    /*for(unsigned i = 0; i < cppli::NUMBER_OF_MINOR_ERROR_TYPES; ++i) {
        cppli::minor_error_behavior(static_cast<cppli::minor_error_type>(i)) = cppli::THROW; // set all minor errors to be fatal
    }

    int result = Catch::Session().run(argc, argv);*/
}
