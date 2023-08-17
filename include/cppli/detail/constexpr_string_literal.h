#pragma once

#include <algorithm>

namespace cppli {
    namespace detail {
        constexpr char underscore_to_hyphen(char c) {
            if (c == '_') {
                return '-';
            }
            else {
                return c;
            }
        }

        constexpr bool isuppercase(char c) {
            switch (c) {
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'G':
                case 'H':
                case 'I':
                case 'J':
                case 'K':
                case 'L':
                case 'M':
                case 'N':
                case 'O':
                case 'P':
                case 'Q':
                case 'R':
                case 'S':
                case 'T':
                case 'U':
                case 'V':
                case 'W':
                case 'X':
                case 'Y':
                case 'Z':
                    return true;
                default:
                    return false;
            }
        }

        constexpr bool islowercase(char c) {
            switch (c) {
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'h':
                case 'i':
                case 'j':
                case 'k':
                case 'l':
                case 'm':
                case 'n':
                case 'o':
                case 'p':
                case 'q':
                case 'r':
                case 's':
                case 't':
                case 'u':
                case 'v':
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                    return true;
                default:
                    return false;
            }
        }

        constexpr bool isletter(char c) {
            return isuppercase(c) || islowercase(c);
        }

        constexpr char tolower(char c) { // yes, this is the only way.
            switch (c) {                  // The C++ standard does not guarantee that 'a', 'b', 'c'... etc. map to contiguous integer values.
                default:
                    return c;
                case 'A':
                    return 'a';
                case 'B':
                    return 'b';
                case 'C':
                    return 'c';
                case 'D':
                    return 'd';
                case 'E':
                    return 'e';
                case 'F':
                    return 'f';
                case 'G':
                    return 'g';
                case 'H':
                    return 'h';
                case 'I':
                    return 'i';
                case 'J':
                    return 'j';
                case 'K':
                    return 'k';
                case 'L':
                    return 'l';
                case 'M':
                    return 'm';
                case 'N':
                    return 'n';
                case 'O':
                    return 'o';
                case 'P':
                    return 'p';
                case 'Q':
                    return 'q';
                case 'R':
                    return 'r';
                case 'S':
                    return 's';
                case 'T':
                    return 't';
                case 'U':
                    return 'u';
                case 'V':
                    return 'v';
                case 'W':
                    return 'w';
                case 'X':
                    return 'x';
                case 'Y':
                    return 'y';
                case 'Z':
                    return 'z';
            }
        }

        constexpr bool is_numeral(char c) {
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    return true;
                default:
                    return false;
            }
        }

        /*std::string tolower_and_convert_underscores(std::string str) {
            for(auto& c : str) {
                if(c == '_') {
                    c = '-';
                }
                else {
                    c = std::tolower(c);
                }
            }

            return std::move(str);
        }*/


    }
    // source for string_literal: https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    template<size_t N>
    struct string_literal {
        template<std::size_t N_>
        constexpr string_literal(const char (& str)[N_]) {
            std::copy(str, str + N, value);
        }

        char value[N];

        constexpr string_literal() = default;

        constexpr string_literal make_lowercase_and_convert_underscores() const {
            string_literal ret;

            for (std::size_t i = 0; i < N; ++i) {
                ret.value[i] = detail::underscore_to_hyphen(detail::tolower(value[i]));
            }

            return ret;
        }

        template<std::size_t N_>
        constexpr bool operator==(const string_literal<N_>& rhs) const {
            bool all_same = true;

            for (std::size_t i = 0; i < std::min(this->size(), rhs.size()); ++i) {
                all_same &= (value[i] == rhs.value[i]);
            }

            return all_same;
        }

        constexpr char operator[](std::size_t i) const {
            return value[i];
        }

        constexpr std::size_t size() const {
            return N;
        }

        std::string string() const {
            std::string ret;
            ret.reserve(N);

            for (std::size_t i = 0; i < N; ++i) {
                ret.push_back(value[i]);
            }
            return ret;
        }

        operator std::string() const {
            return string();
        }
    };

    template<std::size_t N>
    string_literal(const char (&)[N]) -> string_literal<N - 1>;

    namespace detail {
        template<string_literal literal>
        constexpr bool all_lowercase_numeral_or_hyphen() {
            bool all_valid = true;

            for (std::size_t i = 0; i < literal.size(); ++i) {
                all_valid &= (islowercase(literal.value[i]) ||
                              is_numeral(literal.value[i]) ||
                              (literal.value[i] == '-'));
            }

            return all_valid;
        }

        template<string_literal literal>
        constexpr bool contains_uppercase() {
            bool contains = false;

            for (std::size_t i = 0; i < literal.size(); ++i) {
                contains |= isuppercase(literal.value[i]);
            }

            return contains;
        }
    }
}