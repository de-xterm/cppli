#pragma once

#include <algorithm>

namespace cppli::detail {
    constexpr char underscore_to_hyphen(char c) {
        if(c == '_') {
            return '-';
        }
        else {
            return c;
        }
    }

    constexpr char tolower(char c) {
        if((c >= 65) && (c <= 90)) { // is an uppercase char
            return (c+32);
        }
        else {
            return c;
        }
    }

    // source for string_literal: https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char (&str)[N]) {
            std::copy(str, str+N, value);
        }

        char value[N];

        constexpr string_literal() = default;

        constexpr string_literal make_lowercase_and_convert_underscores() const {
            string_literal ret;

            for(std::size_t i = 0; i < N; ++i) {
                ret.value[i] = underscore_to_hyphen(tolower(value[i]));
            }

            return ret;
        }

        operator std::string() const {
            return {make_lowercase_and_convert_underscores().value};
        }

        std::string string() const {
            return *this;
        }
    };

}
