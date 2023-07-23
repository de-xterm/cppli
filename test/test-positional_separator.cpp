
#include <string>

#include "catch_test_macros.hpp"

#ifdef CPPLI_SINGLE_HEADER
    #include "cppli.h"
#else
    #include "cppli/run.h"
    #include "cppli/command_macros.h"
#endif

CPPLI_SUBCOMMAND(posseparatortest,
                 "subcommand for testing the positional separator",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_POSITIONAL(std::string, pos, "positional argument")) {

}

CPPLI_SUBCOMMAND(CPPLI_NAME(posseparatortest, subcommand),
                 "subcommand for testing the positional separator",

                 CPPLI_FLAG(foo, "foo flag", f),
                 CPPLI_POSITIONAL(std::string, pos, "positional argument")) {

}

