#pragma once

#include <cstddef>

#ifdef CPPLI_SINGLE_HEADER
    #include "cppli.h"
#else
    #include "cppli/detail/cli_error.h"
#endif

namespace cppli::detail {
    template<typename T, std::size_t N>
    constexpr std::size_t lengthof(const T(&arr)[N]) {
        return N;
    }
}

#define REQUIRE_THREW(EXCEPTION_TYPE, ERROR_TYPE_ENUM, STATEMENT) \
{                                                                 \
    bool threw = false;                                           \
    try {                                                         \
        STATEMENT;                                                \
    }                                                             \
    catch(EXCEPTION_TYPE& e) {                                    \
        threw = true;                                             \
        REQUIRE(equal(e.error_type(), ERROR_TYPE_ENUM));               \
    }                                                             \
    REQUIRE(threw);                                               \
} (void)0 // force a semicolon

