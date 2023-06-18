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
    
    constexpr bool isupper(char c) {
        switch(c) {
            default: return false;
            case 'A': return true;
            case 'B': return true;
            case 'C': return true;
            case 'D': return true;
            case 'E': return true;
            case 'F': return true;
            case 'G': return true;
            case 'H': return true;
            case 'I': return true;
            case 'J': return true;
            case 'K': return true;
            case 'L': return true;
            case 'M': return true;
            case 'N': return true;
            case 'O': return true;
            case 'P': return true;
            case 'Q': return true;
            case 'R': return true;
            case 'S': return true;
            case 'T': return true;
            case 'U': return true;
            case 'V': return true;
            case 'W': return true;
            case 'X': return true;
            case 'Y': return true;
            case 'Z': return true;
        }
    }

    constexpr char tolower(char c) { // yes, this is the only way.
        switch(c) {                  // The C++ standard does not guarantee that 'a', 'b', 'c'... etc. map to contiguous integer values.
            default: return c;
            case 'A': return 'a';
            case 'B': return 'b';
            case 'C': return 'c';
            case 'D': return 'd';
            case 'E': return 'e';
            case 'F': return 'f';
            case 'G': return 'g';
            case 'H': return 'h';
            case 'I': return 'i';
            case 'J': return 'j';
            case 'K': return 'k';
            case 'L': return 'l';
            case 'M': return 'm';
            case 'N': return 'n';
            case 'O': return 'o';
            case 'P': return 'p';
            case 'Q': return 'q';
            case 'R': return 'r';
            case 'S': return 's';
            case 'T': return 't';
            case 'U': return 'u';
            case 'V': return 'v';
            case 'W': return 'w';
            case 'X': return 'x';
            case 'Y': return 'y';
            case 'Z': return 'z';
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

            for(std::size_t i = 0; i < N-1; ++i) {
                ret.push_back(value[i]);
            }
            return std::move(ret);
        }
    };
    
    template<string_literal literal>
    constexpr bool contains_uppercase() {
        bool contains = false;
        
        for(std::size_t i = 0; i < literal.size(); ++i) {
            contains |= isupper(literal.value[i]);
        }

        return contains;
    }

}
