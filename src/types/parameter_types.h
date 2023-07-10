#pragma once

#include <cstdint>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>

#include "constexpr_string_literal.h"
#include "conversions.h"

namespace cppli {

    namespace detail {
        // placeholder class
        template<string_literal name_, string_literal documentation_, char short_name_ = '\0'>
        class flag {
            static_assert(short_name_ == '\0' || isletter(short_name_), "flag short name must be a letter");

        public:
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;

            flag() = default;
        };

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_ = '\0'>
        class option {
            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");
            static_assert(!optional_ || std::is_constructible_v<type_, std::string> || std::is_move_constructible_v<type_>,
                          "The type parameter of an optional option must be constructible from an std::string or have a move constructor available");


        public:
            using type = type_;
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string;
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = optional_;
            static constexpr auto argument_optional = argument_optional_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;
        };

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                char short_name_>
        class option<type_, name_, documentation_, argument_text_,
                true, true,
                short_name_> {
            /// type_ if default_value == no_default_value, otherwise std::optional<type_>
        private:
            std::optional<type_> value_;
            bool was_included_ = false;

            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");

        public:
            using type = type_;

            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string;
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = true;
            static constexpr auto argument_optional = true;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;


            option(const std::optional<std::string>& str) : was_included_(true) {
                if (str.has_value()) {
                    value_ = conversions::conversion_t<type_>()(*str);
                }
                else {
                    value_ = std::nullopt;
                }
            }

            option() = default;

            option(const option&) = default;
            option(option&&) = default;

            option& operator=(const option&) = default;
            option& operator=(option&&) = default;

            const std::optional<type>& std_optional() const& {
                return value_;
            }

            operator const std::optional<type>&() const& {
                return std_optional();
            }

            explicit operator bool() const {
                return has_value();
            }


            bool was_included() const {
                return was_included_;
            }

            bool has_value() const {
                return value_.has_value();
            }

            /// call func with value if has_value() is true, does nothing otherwise
            template<typename func_t>
            void access_value_if_present(func_t&& func) const {
                if(has_value()) {
                    func(*value_);
                }
            }

            template<typename U>
            type value_or(U&& default_value) const& {
                return value_.template value_or(std::forward<U>(default_value));
            }

            template<typename U>
            constexpr type value_or(U&& default_value) && {
                return value_.template value_or(std::forward<U>(default_value));
            }

            const type_& value() const& {
                return *value_;
            }

            const type_& operator*() const {
                return value();
            }

            const type_* operator->() const {
                return &value();
            }
        };

        template<typename type_, bool optional_, string_literal name_, string_literal documentation_>
        class positional;

        template<typename type_, string_literal name_, string_literal documentation_>
        class positional<type_, true, name_, documentation_> {
            static_assert(std::is_constructible_v<type_, std::string> || std::is_move_constructible_v<type_>,
                          "The type parameter of an optional positional argument must be constructible from an std::string or have a move constructor available");

        public:
            static constexpr auto name = name_;
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string;
            static constexpr auto optional = true;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;
        };

        // this partial specialization is just a wrapper for documentation
        template<typename type_, string_literal name_, string_literal documentation_>
        class positional<type_, false, name_, documentation_> {
        public:
            using type = type_;

            static constexpr auto name = name_;
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string; // TODO: delete one of these
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto optional = false;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;

            positional() = default;
        };

        template<typename type_, string_literal name_, string_literal documentation_>
        class variadic {
        public:
            using type = type_;

            static constexpr auto name = name_;
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string; // TODO: delete one of these
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto optional = false;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;
        };

        template<typename T>
        struct argument_info_t {
            static constexpr bool is_raw_type = true;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = false;
            static constexpr bool has_short_name = false;
        };

        template<string_literal name_, char short_name_, string_literal documentation_>
        struct argument_info_t<flag<name_, documentation_, short_name_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = true;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name  = true;
            static constexpr bool has_short_name = (short_name_ != '\0');
        };

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_>
        struct argument_info_t<option<type_, name_, documentation_, argument_text_, optional_, argument_optional_, short_name_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = true;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = true;
            static constexpr bool has_short_name = (short_name_ != '\0');
        };

        template<typename type_, bool optional_, string_literal name_, string_literal documentation_>
        struct argument_info_t<positional<type_, optional_, name_, documentation_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = true;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = false;
            static constexpr bool has_short_name = false;
        };

        template<typename type_, string_literal name_, string_literal documentation_>
        struct argument_info_t<variadic<type_, name_, documentation_>> {
                static constexpr bool is_raw_type = false;
                static constexpr bool is_flag = false;
                static constexpr bool is_option = false;
                static constexpr bool is_positional = false;
                static constexpr bool is_variadic = true;

                static constexpr bool has_long_name = false;
                static constexpr bool has_short_name = false;
        };

        template<string_literal name_, string_literal documentation_, char short_name_>
        std::ostream& operator<<(std::ostream& os, const detail::flag<name_, documentation_, short_name_>& s) {
            os << *s;
            return os;
        }

        template<typename T, bool optional, string_literal name, string_literal docs>
        std::ostream& operator<<(std::ostream& os, const detail::positional<T, optional, name, docs>& s) {
            os << *s;
            return os;
        }

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_>
        std::ostream& operator<<(std::ostream& os, const detail::option<type_, name_, documentation_, argument_text_, optional_, argument_optional_, short_name_>& s) {
            os << *s;
            return os;
        }
    }
}