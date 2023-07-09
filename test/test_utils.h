#pragma once

#include <cstddef>

namespace cppli::detail {
    template<typename T, std::size_t N>
    constexpr std::size_t lengthof(const T(&arr)[N]) {
        return N;
    }
}

#define REQUIRE_THREW(ERROR_TYPE, STATEMENT) \
{                                            \
    bool threw = false;\
    try {                                   \
        STATEMENT;                                        \
    }                                                       \
    catch(ERROR_TYPE& e) {          \
        threw = true;           \
    }                           \
    REQUIRE(threw);             \
} (void)0 // force a semicolon

