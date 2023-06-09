#pragma once

#include <algorithm>

namespace cppli {
    // source: https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    constexpr char underscore_to_hyphen(char c) {
        if(c == '_') {
            return '-';
        }
        else {
            return c;
        }
    }

    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char (&str)[N]) {
            std::copy(str, str+N, value);
        }

        char value[N];

        constexpr string_literal make_lowercase_and_convert_underscores() {
            string_literal ret;

            for(std::size_t i = 0; i < N; ++i) {
                ret[i] = underscore_to_hyphen(std::tolower(value[i]));
            }

            return ret;
        }

        operator std::string() {
            return {value};
        }
    };

}
