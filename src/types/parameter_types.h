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
        class option;

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_>
        class option {
            /// type_ if default_value == no_default_value, otherwise std::optional<type_>
            using optional_or_raw_type = std::conditional_t<optional_ || argument_optional_, std::optional<type_>, type_>;

        private:
            optional_or_raw_type value_;
            bool was_included_ = false; // TODO: definitely don't have this available if argument is not optional

            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");
            static_assert(!(argument_optional_ && (!optional_)),
                          "required options with optional arguments are not allowed because that wouldn't make sense");

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

            /*operator const type_&() const {
                static_assert((!optional_) && (!argument_optional_),
                              "option implicit conversion to underlying type is only allowed if the option in question is required (not optional) and has a required (not optional) argument");

                return value_;
            }*/

            const std::optional<type>& std_optional() const {
                return value_;
            }

            operator const std::optional<type>&() const {
                return std_optional();
            }

            explicit operator bool() const {
                return has_value();
            }

            option() = default;

            option(const std::optional<std::string>& str) requires(argument_optional_) : was_included_(true) {
                if (str.has_value()) {
                    value_ = conversions::conversion_t<type_>()(*str);
                }
                else {
                    value_ = std::nullopt;
                }
            }

            option(const std::optional<std::string>& str) requires (!argument_optional_)
                    : value_(conversions::conversion_t<type_>()(*str))/*, // emptiness check happens elsewhere
                      was_included_(true)*/ {} // no need to set was_included

            option(const option&) = default;

            option& operator=(const option&) = default;

            option(option&&) = default;

            option& operator=(option&&) = default;

            bool was_included() const {
                static_assert(optional_, // TODO: shouldn't this be optional_ && argument_optional_?
                              "option::was_included is only available if the option in question is optional");

                return was_included_;
            }

            bool has_value() const {
                static_assert(optional_ || argument_optional_,
                              "option::has_value is only available if the option in question is optional or takes an optional argument");

                return value_.has_value();
            }

            /// call func with value if has_value() is true, does nothing otherwise
            template<typename func_t>
            void access_value_if_present(func_t&& func) const {
                static_assert(optional_ || argument_optional_,
                              "option::access_value_if_present is only available if the option in question is optional or takes an optional argument. Use implicit conversion to underlying type instead");

                if (has_value()) {
                    func(*value_);
                }
            }

            type_ value_or(const type_& alternative) const { 
                static_assert(optional_ || argument_optional_,
                              "option::value_or is only available if the option in question is optional or takes an optional argument. Use implicit conversion to underlying type instead");

                return value_.value_or(alternative);
            }

            const type_& get() const {
                if constexpr(optional_) {
                    return *value_;
                }
                else {
                    return value_;
                }
            }

            const type_& operator*() const {
                return get();
            }

            const type_* operator->() const {
                return &get();
            }
        };

        // just a wrapper type for docs
        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                char short_name_>
        class option<type_, name_, documentation_, argument_text_, false, false, short_name_> {
            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");

        public:
            using type = type_;
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string;
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = false;
            static constexpr auto argument_optional = false;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;
        };

        template<typename type_, bool optional_, string_literal name_, string_literal documentation_>
        class positional;

        template<typename type_, string_literal name_, string_literal documentation_>
        class positional<type_, true, name_, documentation_> {

        private:
            std::optional<type_> value_;

        public:
            using type = type_;

            static constexpr auto name = name_;
            static constexpr auto type_string = conversions::conversion_t<type_>::type_string;
            static constexpr auto optional = true;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;


            positional(const std::string& value) : value_(conversions::conversion_t<type_>()(value)) {}
            positional() = default;

            /*operator const type_&() const {
                return value_;
            }*/

            bool has_value() const {
                return value_.has_value();
            }

            /// call func with value if has_value() is true, does nothing otherwise
            template<typename func_t>
            void access_value_if_present(func_t&& func) const {
                if (has_value()) {
                    func(*value_);
                }
            }

            type_ value_or(const type_& alternative) const {
                return value_.value_or(alternative);
            }

            const type_& value() const {
                return *value_;
            }

            const type_& operator*() const {
                return value();
            }

            const type_* operator->() const {
                return &value();
            }
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