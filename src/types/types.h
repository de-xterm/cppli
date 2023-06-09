#pragma once

#include <cstdint>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>

#include "constexpr_string_literal.h"

namespace cppli {

    using int_t    = int;
    using float_t  = float;
    using string_t = std::string;

    namespace detail {
        template<typename T>
        decltype(auto) string_to_type(const std::string& str);

        template<>
        decltype(auto) string_to_type<int_t>(const std::string& str) {
            return std::stoi(str);
        }

        template<>
        decltype(auto) string_to_type<float_t>(const std::string& str) {
            return std::stof(str);
        }

        template<>
        decltype(auto) string_to_type<string_t>(const std::string& str) {
            return str;
        }

        template<typename T>
        struct type_string_t;

        template<>
        struct type_string_t<int_t> {
            static constexpr const char* name = "integer";
        };

        template<>
        struct type_string_t<float_t> {
            static constexpr const char* name = "integer";
        };

        template<>
        struct type_string_t<string_t> {
            static constexpr const char* name = "integer";
        };
    }

    struct no_default_value_t {
        constexpr bool operator==(const no_default_value_t&) const = default;
    };
    static constexpr auto no_default_value = no_default_value_t{};

    struct application_determined_default_value_t {
        constexpr bool operator==(const application_determined_default_value_t&) const = default;
    };
    static constexpr auto application_determined_default_value = application_determined_default_value_t{};


    template<typename type_, string_literal name_, string_literal description_, auto default_value = no_default_value>
    class option {
        /// type_ if default_value == no_default_value, otherwise std::optional<type_>
        using optional_or_raw_type = std::conditional_t<std::is_same_v<std::remove_cvref_t<decltype(default_value)>, no_default_value_t>, type_, std::optional<type_>>;

        optional_or_raw_type value_;

    public:
        operator type_() const {
            static_assert(!std::is_same_v<std::remove_cvref_t<decltype(default_value)>, application_determined_default_value_t>, "option implicit conversion to underlying type is not allowed with application determined default values. You need to use value_or");

            if constexpr(std::is_same_v<decltype(value_), type_>) {
                return value_;
            }
            else {
                return value_.value_or(default_value);
            }
        }

        option(const std::string& str) {
            try {
                value_ = detail::string_to_type<type_>(str);
            }
            catch(std::exception& e) {
                throw std::runtime_error("Could not convert string \"" + str + "\" to option type " + detail::type_string_t<type_>::name + " for option " + name_.make_lowercase_and_convert_underscores());
            }
        }

        template<typename T>
        requires(!std::is_same_v<std::remove_cvref_t<T>, option>)
        option(T&& arg) : value_(std::forward<T>(arg)) {}

        option(const option&) = default;
        option& operator=(const option&) = default;

        option(option&&) = default;
        option& operator=(option&&) = default;


        type_ value_or(const type_& alternative) const {
            static_assert(std::is_same_v<std::remove_cvref_t<decltype(default_value)>, application_determined_default_value_t>, "option::value_or is only available if default value is set to application determined. Use implicit conversion operator instead");
            return value_.value_or(alternative);
        }
    };

}