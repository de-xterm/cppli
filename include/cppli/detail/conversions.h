#pragma once

#include <string>
#include <stdexcept>
#include <limits>
#include <filesystem>

#include "constexpr_string_literal.h"
#include "user_error.h"

namespace cppli {

    // TODO: I think I could move partial specialization operator() implementations into a .cpp file to speed up compile times
    template<typename T>
    struct string_conversion_t {
        T operator()(const std::string& str) const {
            static_assert(std::is_convertible_v<std::string, T> ||
                          std::is_constructible_v<T, std::string>,
                          "if no string_conversion_t partial specialization is available, then std::string must be convertible to the desired type");
            return T(str);
        }

        static constexpr string_literal type_string = T::cppli_type_string;
    };

    template<typename T>
    struct string_conversion_t<std::optional<T>> {
        std::optional<T> operator()(const std::optional<std::string>& s) const {
            if(s.has_value()) {
                if constexpr(std::is_constructible_v<T, const std::string&>) {
                    return std::optional<T>(std::in_place, *s);
                }
                else {
                    return std::optional<T>(string_conversion_t<T>()(*s));
                }
            }
            else {
                return std::nullopt;
            }
        }

        std::optional<T> operator()(const std::string& s) const {
            if constexpr(std::is_constructible_v<T, const std::string&>) {
                return std::optional<T>(std::in_place, s);
            }
            else {
                return std::optional<T>(string_conversion_t<T>()(s));
            }
        }

        static constexpr string_literal type_string = string_conversion_t<T>::type_string;
    };

    template<>
    struct string_conversion_t<int> {
        int operator()(const std::string& str) const {
            try {
                return std::stoi(str);
            }
            catch(std::invalid_argument& e) {
                throw user_error("Could not form a valid integer from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw user_error("Could not form a valid integer from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }
        }

        static constexpr string_literal type_string = "integer";
    };

    template<>
    struct string_conversion_t<unsigned> {
        int operator()(const std::string& str) const {
            unsigned long ret;
            try {
                ret = std::stoul(str);
            }
            catch(std::invalid_argument& e) {
                throw user_error("Could not form a valid integer from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw user_error("Could not form a valid integer from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }
            if(ret > std::numeric_limits<unsigned>::max()) {
                throw user_error("Could not form a valid unsigned integer from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }

            return ret;
        }

        static constexpr string_literal type_string = "integer";
    };

    template<>
    struct string_conversion_t<char> {
        int operator()(const std::string& str) const {
            if(!str.size()) {
                throw user_error("Could not form a character from the given string because it was empty", STRING_CONVERSION_ERROR);
            }

            return str[0];
        }

        static constexpr string_literal type_string = "character";
    };

    template<>
    struct string_conversion_t<float> {
        float operator()(const std::string& str) const {
            try {
                return std::stof(str);
            }
            catch(std::invalid_argument& e) {
                throw user_error("Could not form a valid decimal from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw user_error("Could not form a valid decimal from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
            }
        }

        static constexpr string_literal type_string = "decimal";
    };

    template<>
    struct string_conversion_t<std::string> {
        const std::string& operator()(const std::string& str) const {
            return str;
        }

        static constexpr string_literal type_string = "string";
    };

    template<>
    struct string_conversion_t<std::filesystem::path> {
        std::filesystem::path operator()(const std::string& str) const {
            return {str};
        }

        static constexpr string_literal type_string = "string";
    };
}