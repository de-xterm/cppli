
#include <string>
#include <optional>

#include "catch_test_macros.hpp"

#include "cppli.h"
#include "test_utils.h"

using namespace cppli;
using namespace cppli::detail;

static bool foo_flag;

static std::optional<int> size_option;

/*
CPPLI_SUBCOMMAND(opttest,
                 "takes some flags and some options",
                 CPPLI_FLAG(foo, "foo flag", f)) {

    foo_flag = foo;

    if(size) {
    size_option = *size;
    }
}*/
