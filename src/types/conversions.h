#pragma once

#include <string>
#include <stdexcept>

#include "constexpr_string_literal.h"

namespace cppli {
    namespace conversions {
        template<typename T>
        struct conversion_t {
            T operator()(const std::string& str) const {
                static_assert(std::is_convertible_v<std::string, T> ||
                              std::is_constructible_v<T, std::string>,
                              "if no conversion_t partial specialization is available, then std::string must be convertible to the desired type");
                return T(str);
            }

            static constexpr detail::string_literal type_string = T::cppli_type_string;
        };

        template<>
        struct conversion_t<int> {
            int operator()(const std::string& str) const {
                try {
                    return std::stoi(str);
                }
                catch(std::invalid_argument& e) {
                    throw std::runtime_error("Could not form a valid integer from string \"" + str + "\"");
                }
                catch(std::out_of_range& e) {
                    throw std::runtime_error("Could not form a valid integer from string \"" + str + "\" because the resulting integer would be out of range");
                }
            }

            static constexpr detail::string_literal type_string = "integer";
        };

        template<>
        struct conversion_t<char> {
            int operator()(const std::string& str) const {
                if(!str.size()) {
                    throw std::runtime_error("Could not form a character from the given string because it was empty");
                }

                return str[0];
            }

            static constexpr detail::string_literal type_string = "character";
        };

        template<>
        struct conversion_t<float> {
            float operator()(const std::string& str) const {
                try {
                    return std::stof(str);
                }
                catch(std::invalid_argument& e) {
                    throw std::runtime_error("Could not form a valid decimal from string \"" + str + "\"");
                }
                catch(std::out_of_range& e) {
                    throw std::runtime_error("Could not form a valid decimal from string \"" + str + "\" because the resulting integer would be out of range");
                }
            }

            static constexpr detail::string_literal type_string = "decimal";
        };

        template<>
        struct conversion_t<std::string> {
            const std::string& operator()(const std::string& str) const {
                return str;
            }

            static constexpr detail::string_literal type_string = "string";
        };

    }
}