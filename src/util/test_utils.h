#pragma once

#include <cstddef>

namespace cppli::detail {
    template<typename T, std::size_t N>
    constexpr std::size_t lengthof(const T(&arr)[N]) {
        return N;
    }
}
