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

    template<typename string_t>
    decltype(auto) tolower_and_convert_underscores(string_t&& str) {
        for(auto& c : str) {
            if(c == '_') {
                c = '-';
            }
            else {
                c = std::tolower(c);
            }
        }

        return std::forward<string_t>(str);
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

        template<std::size_t N_>
        constexpr bool operator==(const string_literal<N_>& rhs) const {
            bool all_same = true;

            for(std::size_t i = 0; i < std::min(this->size(), rhs.size()); ++i) {
                all_same &= (value[i] == rhs.value[i]);
            }

            return all_same;
        }

        constexpr std::size_t size() const {
            return N;
        }

        std::string string() const {
            std::string ret;
            ret.reserve(N);

            for(std::size_t i = 0; i < N; ++i) {
                ret.push_back(value[i]);
            }
            return std::move(ret);
        }
    };

}
