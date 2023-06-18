#pragma once

#include <cstdint>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>

#include "constexpr_string_literal.h"

namespace cppli {

   // using string_t = std::string;

    struct string_t {
        std::string val;

        // TODO: set up perfect forwarding
        string_t(const std::string& str) : val(str) {}

        operator const std::string&() const {
            return val;
        }

        static constexpr detail::string_literal string = "string";
    };

    struct int_t {
        int val;

        int_t(const std::string& str) {
            try {
                val = std::stoi(str);
            }
            catch(std::exception& e) {
                throw std::runtime_error("Could not form a valid integer from string \"" + str + '\"');
            }
        }

        operator int() const {
            return val;
        }

        static constexpr detail::string_literal string = "integer";
    };

    struct float_t {
        float val;

        float_t(const std::string& str) {
            try {
                val = std::stof(str);
            }
            catch(std::exception& e) {
                throw std::runtime_error("Could not form a valid integer from string \"" + str + '\"');
            }
        }

        operator float() {
            return val;
        }

        static constexpr detail::string_literal string = "decimal";
    };

    namespace detail {
        struct dummy_t {
        };

        //namespace detail {
        template<typename T>
        struct wrapper_type_info_t {
            static constexpr bool is_wrapper_type = false;

            using wrapped_t = dummy_t;
        };

        template<>
        struct wrapper_type_info_t<int_t> {
            static constexpr bool is_wrapper_type = true;
            using wrapped_t = int;
        };

        template<>
        struct wrapper_type_info_t<float_t> {
            static constexpr bool is_wrapper_type = true;
            using wrapped_t = float;
        };

        template<typename T>
        static constexpr bool is_wrapper_type = wrapper_type_info_t<T>::is_wrapper_type;

        template<typename T>
        using wrapper_type_wrapped_t = typename wrapper_type_info_t<T>::wrapped_t;
        //}

        template<string_literal name_, string_literal documentation_, char short_name_>
        class flag {
            bool value_;

        public:
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;

            flag(bool value) : value_(value) {}

            operator bool() const {
                return value_;
            }
        };

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_ = '\0'>
        class option {
            /// type_ if default_value == no_default_value, otherwise std::optional<type_>
            using optional_or_raw_type = std::conditional_t<optional_ || argument_optional_, std::optional<type_>, type_>;

        private:
            optional_or_raw_type value_;
            bool was_included_ = false;

            static_assert(!(argument_optional_ && (!optional_)),
                          "required options with optional arguments are not allowed because that wouldn't make sense");

        public:
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = type_::string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = optional_;
            static constexpr auto argument_optional = argument_optional_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;

            operator type_() const {
                static_assert((!optional_) && (!argument_optional_),
                              "option implicit conversion to underlying type is only allowed if the option in question is required (not optional) and has a required (not optional) argument");

                return value_;
            }

            operator detail::wrapper_type_wrapped_t<type_>()
            requires(detail::is_wrapper_type<type_>) {
                static_assert((!optional_) && (!argument_optional_),
                              "option implicit conversion to underlying type is only allowed if the option in question is required (not optional) and has a required (not optional) argument");

                if constexpr(detail::is_wrapper_type<type_>) {
                    return value_;
                }
                else {
                    return dummy_t{};
                }
            }

            option() = default;

            option(std::optional<std::string> str) requires(argument_optional_) {
                if (str.has_value()) {
                    value_ = std::move(*str);
                }
                else {
                    value_ = std::nullopt;
                }
            }

            option(std::optional<std::string> str) requires (!argument_optional_)
                    : value_(std::move(*str)), // emptiness check happens elsewhere
                      was_included_(true) {}

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
        };

        template<typename type_, bool optional_, string_literal name_, string_literal documentation_>
        class positional {
            using optional_or_raw_type = std::conditional_t<optional_, std::optional<type_>, type_>;

        private:
            optional_or_raw_type value_;

        public:
            static constexpr auto name = name_;
            static constexpr auto type_string = type_::string;
            static constexpr auto optional = optional_;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;


            positional(std::string value) : value_(std::move(value)) {}

            operator type_() const {
                static_assert(!optional_,
                              "positional implicit conversion to underlying type is only allowed if the positional argument in question is required (not optional)");

                return value_;
            }

            operator detail::wrapper_type_wrapped_t<type_>() const requires(detail::is_wrapper_type<type_>) {
                static_assert(!optional_,
                              "positional implicit conversion to underlying type is only allowed if the positional argument in question is required (not optional)");

                if constexpr(detail::is_wrapper_type<type_>) {
                    return value_;
                } else {
                    return dummy_t{};
                }
            }

            bool has_value() const {
                static_assert(optional_,
                              "positional::has_value is only available if the positional argument in question is optional");

                return value_.has_value();
            }

            /// call func with value if has_value() is true, does nothing otherwise
            template<typename func_t>
            void access_value_if_present(func_t&& func) const {
                static_assert(optional_,
                              "positional::access_value_if_present is only available if the positional argument in question is optional");

                if (has_value()) {
                    func(*value_);
                }
            }

            type_ value_or(const type_& alternative) const {
                static_assert(optional_,
                              "positional::value_or is only available if the positional argument in question is optional");

                return value_.value_or(alternative);
            }
        };

        template<typename T>
        struct argument_info_t;

        template<string_literal name_, char short_name_, string_literal documentation_>
        struct argument_info_t<flag<name_, documentation_, short_name_>> {
            static constexpr bool is_flag = true;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = false;
        };

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_>
        struct argument_info_t<option<type_, name_, documentation_, argument_text_, optional_, argument_optional_, short_name_>> {
            static constexpr bool is_flag = false;
            static constexpr bool is_option = true;
            static constexpr bool is_positional = false;
        };

        template<typename type_, bool optional_, string_literal name_, string_literal documentation_>
        struct argument_info_t<positional<type_, optional_, name_, documentation_>> {
            static constexpr bool is_flag = false;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = true;
        };
    }
}