
#include "catch_session.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #define CPPLI_IMPLEMENTATION
#endif

#include "cppli.h"

int main(int argc, char** argv) {
    int result = Catch::Session().run(argc, argv);
}
