#pragma once

#include <cstddef>

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
        REQUIRE(e.error_type() == ERROR_TYPE_ENUM);               \
    }                                                             \
    REQUIRE(threw);                                               \
} (void)0 // force a semicolon

