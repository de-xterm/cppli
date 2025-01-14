
#include "catch_session.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #define CPPLI_IMPLEMENTATION
    #include "cppli.h"
#else
    #include "cppli/configuration.h"
    #include "cppli/run.h"
#endif

int main(int argc, char** argv) {
    /*try {
        cppli::run<"program", "does stuff">(argc, argv);
    }
    catch(cppli::user_error& e) {
        std::cerr << e << '\n';
    }*/

    for(unsigned i = 0; i < cppli::NUMBER_OF_MINOR_ERROR_TYPES; ++i) {
        cppli::set_minor_error_behavior(static_cast<cppli::minor_error_type>(i),cppli::THROW); // set all minor errors to be fatal
    }

    return Catch::Session().run(argc, argv);
}
