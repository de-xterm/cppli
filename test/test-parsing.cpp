
#include "catch_test_macros.hpp"

#include "arg_parsing.h"
#include "cppli.h"

CPPLI_SUBCOMMAND(CPPLI_NAME(repo, init),
                 CPPLI_FLAG(force_reset, "Force the thing to rest the thing", f)) {

}


TEST_CASE("arg parsing works") {
    //const char* args[] = {"testcommand", "subcommand", "-f", "--no-backups", ""};
    //auto commands = cppli::detail::parse(3, args);
}