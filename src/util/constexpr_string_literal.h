#pragma once

#include <algorithm>

namespace cppli::detail {

    // source: https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char (&str)[N]) {
            std::copy(str, str+N, value);
        }

        char value[N];
    };
}
