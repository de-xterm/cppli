#pragma once



//included from file "subcommand_macros.h"

namespace cppli::detail {
    struct evaluate_at_file_scope_dummy_t {
        template<typename T>
        evaluate_at_file_scope_dummy_t(T&&) {}
    };

    #define cppli_internal_PARENS ()

    #define cppli_internal_EXPAND(...) cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(__VA_ARGS__))))
    /*#define cppli_internal_EXPAND4(...) cppli_internal_EXPAND3(cppli_internal_EXPAND3(cppli_internal_EXPAND3(cppli_internal_EXPAND3(__VA_ARGS__))))
    #define cppli_internal_EXPAND3(...) cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(cppli_internal_EXPAND2(__VA_ARGS__))))*/
    #define cppli_internal_EXPAND2(...) cppli_internal_EXPAND1(cppli_internal_EXPAND1(cppli_internal_EXPAND1(cppli_internal_EXPAND1(__VA_ARGS__))))
    #define cppli_internal_EXPAND1(...) __VA_ARGS__

    #define cppli_internal_FOR_EACH(macro, ...)                                    \
          __VA_OPT__(cppli_internal_EXPAND(cppli_internal_FOR_EACH_HELPER(macro, __VA_ARGS__)))

    #define cppli_internal_FOR_EACH_HELPER(macro, a1, ...)                                    \
          macro(a1)                                                                           \
          __VA_OPT__(cppli_internal_FOR_EACH_AGAIN cppli_internal_PARENS (macro, __VA_ARGS__))

    #define cppli_internal_FOR_EACH_AGAIN() cppli_internal_FOR_EACH_HELPER


    #define cppli_internal_STRINGIFY(S) cppli_internal_STRINGIFY_impl(S)
    #define cppli_internal_STRINGIFY_impl(S) #S

    #define cppli_internal_STRINGIFY_WITH_COMMA(S) cppli_internal_STRINGIFY(S),

    #define ONE_ARG_CAT(arg1) arg1

    #define NOT_ONE_ARG_CAT(...) \
        cppli_internal_EXPAND(cppli_internal_NOT_FINAL_CAT_HELPER(__VA_ARGS__))

    #define cppli_internal_CAT(arg1, ...)                                    \
        __VA_OPT__(NOT_)##ONE_ARG_CAT(arg1 __VA_OPT__(, __VA_ARGS__))

    #define cppli_internal_NOT_FINAL_CAT_HELPER(a1, a2, ...)                         \
          __VA_OPT__(NOT_)##FINAL_CAT_AGAIN cppli_internal_PARENS (a1##_##a2 __VA_OPT__(, __VA_ARGS__))

    #define FINAL_CAT_HELPER(arg) arg

    #define FINAL_CAT_AGAIN() FINAL_CAT_HELPER
    #define NOT_FINAL_CAT_AGAIN() cppli_internal_NOT_FINAL_CAT_HELPER

    #define cppli_internal_UNIQUE_NAME cppli_internal_CAT(name, __COUNTER__)

    #define cppli_internal_EVALUATE_AT_FILE_SCOPE(EXPR)                        \
    namespace {                                                 \
        ::cppli::detail::evaluate_at_file_scope_dummy_t cppli_internal_UNIQUE_NAME = EXPR;      \
    }
}
//end of "subcommand_macros.h" include

//included from file "arg_parsing.h"


//included from file "subcommand.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <optional>
#include <set>
#include <cassert>
#include <iostream>


//included from file "parameter_types.h"

#include <cstdint>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>


//included from file "constexpr_string_literal.h"

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

    constexpr bool isuppercase(char c) {
        switch(c) {
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
            case 'Z': return true;
            default: return false;
        }
    }

    constexpr bool islowercase(char c) {
        switch(c) {
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
            case 'z': return true;
            default: return false;
        }
    }

    constexpr bool isletter(char c) {
        return isuppercase(c) || islowercase(c);
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

    constexpr bool is_numeral(char c) {
        switch(c) {
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
            default: return false;
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
        template<std::size_t N_>
        constexpr string_literal(const char (&str)[N_]) {
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
            return ret;
        }
    };

    template<std::size_t N>
    string_literal(const char (&)[N]) -> string_literal<N-1>;

    template<string_literal literal>
    constexpr bool all_lowercase_numeral_or_hyphen() {
        bool all_valid = true;

        for(std::size_t i = 0; i < literal.size(); ++i) {
            all_valid &= (islowercase(literal.value[i]) ||
                          is_numeral(literal.value[i])  ||
                          (literal.value[i] == '-'));
        }

        return all_valid;
    }

    template<string_literal literal>
    constexpr bool contains_uppercase() {
        bool contains = false;

        for(std::size_t i = 0; i < literal.size(); ++i) {
            contains |= isuppercase(literal.value[i]);
        }

        return contains;
    }

}

//end of "constexpr_string_literal.h" include

//included from file "conversions.h"

#include <string>
#include <stdexcept>


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
//end of "conversions.h" include

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
            bool was_included_ = false;

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

            operator const type_&() const {
                static_assert((!optional_) && (!argument_optional_),
                              "option implicit conversion to underlying type is only allowed if the option in question is required (not optional) and has a required (not optional) argument");

                return value_;
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

            operator const type_&() const {
                return value_;
            }

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

            const type_& get() const {
                return *value_;
            }

            const type_& operator*() const {
                return get();
            }

            const type_* operator->() const {
                return &get();
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

        template<typename T>
        struct raw_type_if_required_helper_t {
            using type = T;
        };

        template<typename type_, string_literal name_, string_literal documentation_>
        struct raw_type_if_required_helper_t<positional<type_, false, name_, documentation_>> {
            using type = typename type_::type;
        };

        template<typename type_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                char short_name_>
        struct raw_type_if_required_helper_t<option<type_, name_, documentation_, argument_text_, false, false, short_name_>> {
            using type = typename type_::type;
        };

        template<typename T>
        using raw_type_if_required_t = typename raw_type_if_required_helper_t<std::remove_cvref_t<T>>::type;

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
//end of "parameter_types.h" include

namespace cppli::detail {
    struct subcommand_inputs_t {
        std::vector<std::string> positional_args;
        std::unordered_map<std::string, std::optional<std::string>> options_to_values;
        std::unordered_set<std::string> flags;

        bool is_empty() const;
    };

    using subcommand_name_t = std::vector<std::string>;

    /// this is just boost::hash_combine, but I don't want to drag boost into this library just for hash_combine
    template<typename T>
    std::size_t hash_combine(std::size_t& seed, const T& val) {
        return (seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }

    struct subcommand_name_hash_t {
        std::size_t operator()(const subcommand_name_t& name) const noexcept;
    };

    struct subcommand_t {
        subcommand_name_t name;
        subcommand_inputs_t inputs;
    };

    using subcommand_func_t = void(*)(const subcommand_t&);

    struct subcommand_inputs_info_t {
        std::unordered_set<std::string> flags;
        std::unordered_map<std::string, bool> option_argument_is_optional;
    };

    std::unordered_map<subcommand_name_t, subcommand_inputs_info_t, subcommand_name_hash_t>&   subcommand_name_to_inputs_info();
    std::unordered_map<subcommand_name_t, subcommand_func_t,        subcommand_name_hash_t>&   subcommand_name_to_func();

    /// if arg appended to parent_command_names forms a valid subcommand,
    /// pushes back arg to parent_command_names and returns true.
    /// Otherwise, just returns false
    bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg);

    bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name);
    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name);

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name);

    bool is_namespace(const subcommand_name_t& subcommand);

    void set_program_name_and_description(std::string&& name, std::string&& description); // todo: these should probably be in documentation.h/cpp

    const std::string& program_name();
    const std::string& program_description();

    bool main_command_is_namespace();


}
//end of "subcommand.h" include

namespace cppli::detail {
    struct parse_ret_t {
        std::vector<subcommand_t> subcommands;
        bool printed_help;
    };

    parse_ret_t parse(int argc, const char* const* const argv);
}
//end of "arg_parsing.h" include

//included from file "command_registration.h"


//included from file "documentation.h"

#include <string>


namespace cppli {
    enum documentation_verbosity {
        NAME_ONLY,
        NAME_AND_DESCRIPTION,
        NAME_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS
    };

    extern documentation_verbosity default_help_verbosity;
    extern unsigned default_help_recursion_level;

    namespace detail {
        struct flag_documentation_t {
            std::string name,
                        documentation;

            char short_name;

            flag_documentation_t(const std::string& name, const std::string& documentation, char short_name);

            bool operator<(const flag_documentation_t& rhs) const;
        };

        struct option_documentation_t {
            std::string type,
                        name,
                        argument_text,
                        documentation;

            char short_name;

            bool is_optional,
                 argument_is_optional;

            option_documentation_t(const std::string& type, const std::string& name, const std::string& argument_text,
                                   const std::string& documentation, char short_name, bool is_optional, bool argument_is_optional);

            bool operator<(const option_documentation_t& rhs) const;
        };

        struct positional_documentation_t {
            std::string type,
                        name,
                        documentation;

            positional_documentation_t(const std::string& type, const std::string& name, const std::string& documentation,
                                       bool optional);

            bool optional;
        };

        struct variadic_documentation_t {
            std::string type,
                        name,
                        documentation;

            variadic_documentation_t(const std::string& type, const std::string& name, const std::string& documentation);
        };

        struct subcommand_documentation_t {
            std::string  name; // this is what we're sorting by
            std::string description;

            std::set<flag_documentation_t>          flags; // using ordered set because we want to print commands alphabetically
            std::set<option_documentation_t>        options;
            std::vector<positional_documentation_t> positionals;
            std::optional<variadic_documentation_t> variadic; // not vector because only one is allowed

            std::set<std::string>          subcommands;

            bool is_namespace = true;

            subcommand_documentation_t() = default;

            subcommand_documentation_t(const std::string& name, const char* description);

            bool operator<(const subcommand_documentation_t& rhs) const;
        };

        std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs();
        std::set<std::string>& top_level_subcommands(); // TODO: remove this if it doesn't turn out to be necessary
    }

    std::string get_documentation_string(const detail::subcommand_name_t&, documentation_verbosity verbosity, unsigned recursion);

    /// returns documentation for the main command
    std::string get_documentation_string(documentation_verbosity verbosity, unsigned max_recursion_level);

}
//end of "documentation.h" include

namespace cppli::detail {
    template<typename T, typename last>
    /*add_const_ref_if_string_t<T>::*/T process_argument(unsigned cumulative_positional_index, const subcommand_t& subcommand) {
        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument

        std::string main_command_or_subcommand;

        if(subcommand.name == subcommand_name_t{"MAIN"}) {
            main_command_or_subcommand = "main command";
        }
        else {
            main_command_or_subcommand = "subcommand";
        }

        if constexpr(arg_info_t::is_raw_type) {
            std::string canonical_name = last::name.string();

            std::optional<std::string> short_name;

            if constexpr(last::has_short_name) {
                short_name = std::string{last::short_name};
            }

            if constexpr(argument_info_t<last>::is_flag) {
                return subcommand.inputs.flags.contains(last::name.string()) ||
                       (short_name && subcommand.inputs.flags.contains(std::string{last::short_name}));
            }
            else if constexpr(argument_info_t<last>::is_option) {
                if(!subcommand.inputs.options_to_values.contains(canonical_name) &&
                   (short_name && !subcommand.inputs.options_to_values.contains(*short_name))) {

                    throw std::runtime_error(main_command_or_subcommand + " \""  + subcommand_name_to_docs()[subcommand.name].name + "\" was not provided with required option \"" + canonical_name + '"');
                }
                else if(!subcommand.inputs.options_to_values.at(canonical_name).has_value() ||
                        (short_name && !subcommand.inputs.options_to_values.at(*short_name).has_value())) {

                    throw std::runtime_error(main_command_or_subcommand + " \""  + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\" requires an argument, but one was not provided "
                                                                                                                                                                       "(expected an argument of type " + conversions::conversion_t<T>::type_string.make_lowercase_and_convert_underscores().string() + ')');
                }
                else {
                    if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                        try {
                            return conversions::conversion_t<T>()(subcommand.inputs.options_to_values.at(canonical_name));
                        }
                        catch(std::runtime_error& e) {
                            throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\". Details: " + e.what());
                        }
                    }
                    else { // has_short_name is guaranteed to be true at this point
                        try {
                            return conversions::conversion_t<T>()(subcommand.inputs.options_to_values.at(*short_name));
                        }
                        catch(std::runtime_error& e) {
                            throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + *short_name + "\" (full name \"." + canonical_name + "\"). Details: " + e.what());
                        }
                    }
                }
            }
            else if constexpr(argument_info_t<last>::is_positional) { // positional
                --cumulative_positional_index; // because we've already skipped over the actual parameter for the positional, cumulative_positional_index will be too big
                if((cumulative_positional_index >= subcommand.inputs.positional_args.size())) {
                    throw std::runtime_error(main_command_or_subcommand + " \""  + subcommand_name_to_docs()[subcommand.name].name + "\" required positional argument \"" + canonical_name + "\" was not provided (expected an argument of type " + conversions::conversion_t<T>::type_string.string() + ')');
                }
                else {
                    try {
                        return conversions::conversion_t<T>()(subcommand.inputs.positional_args[cumulative_positional_index]);
                    }
                    catch(std::runtime_error& e) {
                        throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" positional argument \"" + canonical_name + "\". Details: " + e.what());
                    }
                }
            }
            else { // variadic
                T ret; // T is a vector
                for(unsigned i = cumulative_positional_index; i < subcommand.inputs.positional_args.size(); ++i) {
                    try {
                        ret.emplace_back(conversions::conversion_t<typename last::type>()(subcommand.inputs.positional_args[i]));
                    }
                    catch(std::runtime_error& e) {
                        throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" variadic argument pack \"" + canonical_name + "\" at argument index " + std::to_string(i) + ". Details: " + e.what());
                    }
                }
                return ret;
            }
        }
        else {
            std::string canonical_name = T::name.string();

            std::optional<std::string> short_name;

            if constexpr(T::has_short_name) {
                short_name = std::string{T::short_name};
            }

            if constexpr(arg_info_t::is_option) {
                if constexpr(T::optional) {
                    if constexpr(T::argument_optional) {
                        if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                            try {
                                return {subcommand.inputs.options_to_values.at(canonical_name)}; // no need for has_value check here; returning an empty optional is valid
                            }
                            catch(std::runtime_error& e) {
                                throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\". Details: " + e.what());
                            }
                        }
                        else if(short_name && subcommand.inputs.options_to_values.at(*short_name)) {
                            try {
                                return {subcommand.inputs.options_to_values.at(*short_name)}; // no need for has_value check here; returning an empty optional is valid
                            }
                            catch(std::runtime_error& e) {
                                throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + *short_name + "\" (full name \"." + canonical_name + "\"). Details: " + e.what());
                            }
                        }
                        else {
                            return {}; // default constructor for optional option with optional argument inits to empty optional and was_included to false
                        }
                    }
                    else {
                        if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                            if(!subcommand.inputs.options_to_values.at(canonical_name).has_value()) {
                                throw std::runtime_error(main_command_or_subcommand + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\" requires an argument, but one was not provided (expected an argument of type " + static_cast<std::string>(conversions::conversion_t<T>::type_string.make_lowercase_and_convert_underscores()) + "."
                                                                                                                                                                                                                                                                                                                                                   "Note that this option is optional, so it is valid to omit it entirely, but the option's argument is required, so if the option is provided, it must come with an argument");
                            }
                            else {
                                try {
                                    return {subcommand.inputs.options_to_values.at(canonical_name)}; // no need for has_value check here; returning an empty optional is valid
                                }
                                catch(std::runtime_error& e) {
                                    throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\". Details: " + e.what());
                                }
                            }
                        }
                        else if(short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
                            if(!subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                                throw std::runtime_error(main_command_or_subcommand + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + *short_name + "\" (full name \"." + canonical_name + "\") requires an argument, but one was not provided (expected an argument of type " + conversions::conversion_t<T>::type_string.string() + "."
                                                                                                                                                                                                                                                                                                                              "Note that this option is optional, so it is valid to omit it entirely, but the option's argument is required, so if the option is provided, it must come with an argument");
                            }
                            else {
                                try {
                                    return {subcommand.inputs.options_to_values.at(*short_name)}; // no need for has_value check here; returning an empty optional is valid
                                }
                                catch(std::runtime_error& e) {
                                    throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + *short_name + "\" (full name \"." + canonical_name + "\"). Details: " + e.what());
                                }
                            }
                        }
                        else {
                            return {};
                        }
                    }
                }
                else {
                    return {}; // this variable is a dummy
                }
            }
            else if constexpr(arg_info_t::is_flag) {
                return {}; // dummy
            }
            else if constexpr(arg_info_t::is_positional) {
                if constexpr(T::optional) {
                    if((cumulative_positional_index >= subcommand.inputs.positional_args.size())) {
                        return {};
                    }
                    else {
                        try {
                            return {subcommand.inputs.positional_args[cumulative_positional_index]};
                        }
                        catch(std::runtime_error& e) {
                            throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" positional argument \"" + canonical_name + "\". Details: " + e.what());
                        }
                    }
                }
                else {
                    return {}; // dummy
                }
            }
            else { // is variadic
                return {};
            }
        }
    }

    template<typename T_>
    struct type_wrapper { using T = T_; };

    template<std::size_t current_index, std::size_t desired_index, typename T, typename...Ts>
    static constexpr auto get_type_from_index_in_pack_func() {
        if constexpr((current_index == 0) && (desired_index > sizeof...(Ts))) {
            return type_wrapper<void>{};
        }
        else if constexpr(current_index == desired_index) {
            return type_wrapper<T>{};
        }
        else {
            //static_assert(sizeof...(Ts) > 0, "index out of range");
            return get_type_from_index_in_pack_func<current_index+1, desired_index, Ts...>();
        }
    }

    template<std::size_t index, typename...Ts>
    using get_type_from_index_in_pack = typename decltype(get_type_from_index_in_pack_func<0,index, Ts...>())::T;

    template<std::size_t current_index, std::size_t max_index, typename arg_t, typename...arg_ts>
    constexpr std::size_t count_positionals_before_index_func() {
        if constexpr(current_index < max_index) {
            return count_positionals_before_index_func<current_index+1, max_index, arg_ts...>()+argument_info_t<std::remove_cvref_t<arg_t>>::is_positional;
        }
        else {
            return 0; // return 0 instead of is_positional because inclusivity is not what we want
        }
    }

    // TODO: static_assert for optional positions followed by required positionals
    template<bool optional_positional_enountered, typename arg_t, typename...arg_ts>
    constexpr std::size_t count_positionals() {
        using T = std::remove_cvref_t<arg_t>;
        using arg_info = argument_info_t<T>;

        constexpr bool is_positional = arg_info::is_positional;
        if constexpr(is_positional && optional_positional_enountered) {
            static_assert(T::optional, "required positionals cannot follow optional positionals");
        }

        if constexpr(sizeof...(arg_ts) > 0) {
            if constexpr(is_positional) {
                return count_positionals<optional_positional_enountered || T::optional, arg_ts...>()+is_positional;
            }
            else {
                return count_positionals<optional_positional_enountered, arg_ts...>()+is_positional;
            }
        }
        else {
            return is_positional;
        }
    }

    template<typename arg_t, typename...arg_ts>
    constexpr bool pack_contains_variadic() {
        if constexpr(argument_info_t<arg_t>::is_variadic) {
            return true;
        }
        else {
            if constexpr(sizeof...(arg_ts) > 0) {
                return pack_contains_variadic<arg_ts...>();
            }
            else {
                return false;
            }
        }
    }

    template<std::size_t max_index, typename...arg_ts>
    static constexpr std::size_t count_positionals_before_index_v = count_positionals_before_index_func<0, max_index, arg_ts...>();

    template<typename func_t, auto func, typename index_seq>
    struct call_func_wrapper_impl_t;

    template<typename return_t, typename...arg_ts, std::size_t...indices, auto func>
    struct call_func_wrapper_impl_t<return_t(*)(arg_ts...), func, std::integer_sequence<std::size_t, indices...>> {
        static void call_func(const subcommand_t& subcommand) {
            func(process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>, std::remove_cvref_t<get_type_from_index_in_pack<(indices)-1, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand)...);
        }
    };

    template<typename func_t, auto func>
    struct call_func_wrapper_t;

    template<typename return_t, typename...arg_ts, auto func>
    struct call_func_wrapper_t<return_t(*)(arg_ts...), func> { // we need all these ugly partial specializations so that we can deduce arg_ts and indices
        static void call_func(const subcommand_t& subcommand) {
            if constexpr(!pack_contains_variadic<std::remove_cvref_t<arg_ts>...>()) {
                constexpr auto positionals_count = count_positionals<false, arg_ts...>();
                if(positionals_count < subcommand.inputs.positional_args.size()) {
                    std::cerr << "Too many positional arguments given to " << (subcommand.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << " \"" << subcommand_name_to_docs()[subcommand.name].name <<
                              "\" (expected " << std::to_string(positionals_count) << ", got " << std::to_string(subcommand.inputs.positional_args.size()) << "). Excess positional argument will be ignored\n";
                }
            }
            call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::call_func(subcommand);
        }
    };

    template<auto func>
    struct call_func_wrapper_t<void(*)(), func> { // we need all this ugly partial specializations so that we can deduce arg_ts and indices
        static void call_func(const subcommand_t& subcommand) {
            func();
        }
    };

    template<auto func>
    void call_func(const subcommand_t& command) {
        call_func_wrapper_t<decltype(func), func>::call_func(command);
    }

    template<typename T, typename U, typename...Ts>
    constexpr bool pack_contains_short_name_func() {
        if constexpr(argument_info_t<T>::has_short_name && argument_info_t<U>::has_short_name) {
            if constexpr(sizeof...(Ts) > 0) {
                return (T::short_name == U::short_name) || pack_contains_short_name_func<T, Ts...>();
            }
            else {
                return (T::short_name == U::short_name);
            }
        }
        else {
            return false;
        }
    }

    template<std::size_t index, typename first_t, typename...rest_ts>
    constexpr bool no_repeated_short_names_func() {
        if constexpr(sizeof...(rest_ts) == 0) {
            return true;
        }
        else if constexpr(index < sizeof...(rest_ts)) {
            return (!pack_contains_short_name_func<first_t, rest_ts...>()) && no_repeated_short_names_func<index+1, rest_ts..., first_t>();
        }
        else {
            return (!pack_contains_short_name_func<first_t, rest_ts...>());
        }
    }

    template<typename...arg_ts>
    constexpr bool no_repeated_short_names_v = no_repeated_short_names_func<0, std::remove_cvref_t<arg_ts>...>();


    template<typename T, typename U, typename...Ts>
    constexpr bool pack_contains_long_name_func() {
        if constexpr(argument_info_t<T>::has_long_name && argument_info_t<U>::has_long_name) {
            if constexpr(sizeof...(Ts) > 0) {
                return (T::name == U::name) || pack_contains_short_name_func<T, Ts...>();
            }
            else {
                return (T::name == U::name);
            }
        }
        else {
            return false;
        }
    }

    template<std::size_t index, typename first_t, typename...rest_ts>
    constexpr bool no_repeated_long_names_func() {
        if constexpr(sizeof...(rest_ts) == 0) {
            return true;
        }
        else if constexpr(index < sizeof...(rest_ts)) {
            return (!pack_contains_long_name_func<first_t, rest_ts...>()) && no_repeated_long_names_func<index+1, rest_ts..., first_t>();
        }
        else {
            return (!pack_contains_long_name_func<first_t, rest_ts...>());
        }
    }

    template<typename...arg_ts>
    constexpr bool no_repeated_long_names_v = no_repeated_long_names_func<0, std::remove_cvref_t<arg_ts>...>();

    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(arg_t, arg_ts...) = nullptr) {
        using arg_info_t = argument_info_t<std::remove_cvref_t<arg_t>>;

        if constexpr(!arg_info_t::is_raw_type) {
            using type = std::remove_cvref_t<arg_t>;

            static_assert(no_repeated_short_names_v<arg_t, arg_ts...>, "multiple flags/options cannot share a short name");
            static_assert(no_repeated_long_names_v<arg_t, arg_ts...>,  "multiple flags/options cannot share a long name");

            if constexpr(arg_info_t::is_flag) {
                documentation.flags.emplace(type::name.string(),
                                            type::documentation.string(),
                                            type::short_name);

                info.flags.insert(type::name.string());

                if(type::short_name) {
                    info.flags.insert(std::string{type::short_name});
                }
            }
            else if constexpr(arg_info_t::is_option) {
                documentation.options.emplace(type::type_string.string(),
                                              type::name.string(),
                                              type::argument_text.string(),
                                              type::documentation.string(),
                                              type::short_name,

                                              type::optional,
                                              type::argument_optional);

                info.option_argument_is_optional.emplace(type::name.string(), type::argument_optional);

                if constexpr(type::short_name != '\0') {
                    info.option_argument_is_optional.emplace(std::string{type::short_name}, type::argument_optional);
                }
            }
            else if constexpr(arg_info_t::is_positional) { // positional
                documentation.positionals.emplace_back(type::type_string.string(),
                                                       type::name.string(),
                                                       type::documentation.string(),

                                                       type::optional);
            }
            else { // variadic
                documentation.variadic = variadic_documentation_t{type::type_string.string(),
                                          type::name.string(),
                                          type::documentation.string()};
            }
        }

        if constexpr(sizeof...(arg_ts) > 0) {
            generate_input_info_and_docs<return_t, arg_ts...>(info, documentation);
        }
    }

    inline void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, void(*func)() = nullptr) {
        // do nothing
    }

    struct dummy_t{};

    template<auto func>
    dummy_t register_subcommand(const subcommand_name_t& name, const char* description) {
        subcommand_inputs_info_t   info;
        subcommand_documentation_t docs(name.back(), description);

        subcommand_name_t cumulative_name;
        for(unsigned i = 0; i < name.size()-1; ++i) {
            cumulative_name.push_back(name[i]);
            if(!subcommand_name_to_docs().contains(cumulative_name)) {
                subcommand_name_to_docs()[cumulative_name].name = cumulative_name.back();
            }
            subcommand_name_to_docs()[cumulative_name].subcommands.emplace(name[i+1]);
        }

        /*if(name.front() != "MAIN") {
            subcommand_name_to_docs()[{"MAIN"}].subcommands.emplace(name.front());
        }*/

        generate_input_info_and_docs(info, docs, func);

        subcommand_name_to_func().emplace(name, call_func<func>);
        subcommand_name_to_inputs_info().emplace(name, std::move(info));

        if(name == subcommand_name_t{"MAIN"}) {
            subcommand_name_to_docs()[name].flags = std::move(docs.flags);
            subcommand_name_to_docs()[name].options = std::move(docs.options);
            subcommand_name_to_docs()[name].positionals = std::move(docs.positionals);
            subcommand_name_to_docs()[name].variadic = std::move(docs.variadic);

            subcommand_name_to_docs()[name].is_namespace = false;

            // can't do a full assignment because that would wipe subcommands
        }
        else {
            subcommand_name_to_docs().insert_or_assign(name, std::move(docs));
        }

        return {};
    }
}
//end of "command_registration.h" include

#ifdef CPPLI_IMPLEMENTATION

//included from file "arg_parsing.cpp"

#include <stdexcept>
#include <cassert>
#include <iostream>


namespace cppli::detail {
    parse_ret_t parse(int argc, const char* const* const argv) {

        if(argc == 0) {
            std::cerr << "argc == 0. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }
        if(!argv[0]) {
            std::cerr << "argv[0] was null. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }

        /*if(program_name() != argv[0]) { // TODO: switch to enable/disable this warning. I'm commenting it out for now because it's annoying
            std::cerr << "The name used to invoke the program (\"" << argv[0] << "\") was not the same as the program's documented name (\"" << program_name() << "\")\n";
        }*/

        // skip the program name
        subcommand_name_t subcommand_name = {"MAIN"};

        subcommand_inputs_t args;

        std::vector<subcommand_t> commands{{subcommand_name, args}};

        bool disambiguate_next_arg = false;

        bool invalid_input_to_namespace = false;

        std::string first_command_name = argv[0];

        bool in_namespace = is_namespace({"MAIN"});
        //commands.push_back({{program_name()}, args});

        std::string command_or_subcommand = "command";

        std::string current_subcommand_name_string = argv[0];


        for(unsigned arg_i = 1; arg_i < argc; ++arg_i) {
            std::string arg_string = argv[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string) && !disambiguate_next_arg) {
                in_namespace = is_namespace(subcommand_name);

                command_or_subcommand = "subcommand";
                current_subcommand_name_string = arg_string;

                commands.back().inputs = std::move(args);
                args = {};

                commands.push_back({subcommand_name});
            }
            else {
                /*if(in_namespace) { // I forgot why this doesn't work
                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                    continue;
                }*/
                if((arg_string.substr(0,2) == "--") && !disambiguate_next_arg) { // long flag (these are ez)
                                                    // we need this check so that we can handle the case where "--" is the thing we're trying to disambiguate. Ex: "program -- --"
                    if((arg_string.size() == 2) /*&& (!disambiguate_next_arg)*/) { // if the whole string is just "--", then this arg is used to disambiguate the next
                        disambiguate_next_arg = true; // ( "--" just means "the next arg is positional, even if it looks like an option/flag (starts with '-' or "--"), or a subcommand (matches a subcommand name))
                    }
                    else {
                        std::string::size_type equals_pos;
                        if((equals_pos = arg_string.find('=')) != std::string::npos) { // TODO: make sure the -2 doesn't cause issues
                            std::string option_name  = arg_string.substr(2, equals_pos-2),
                                    option_value = arg_string.substr(equals_pos+1, arg_string.size()-(equals_pos+1));

                            if(subcommand_takes_option(subcommand_name, option_name)) {
                                args.options_to_values.emplace(option_name, option_value);
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_name)) {
                                std::cerr << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << option_name << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \"" << arg_string << "\"). "
                                             "The value will be ignored and the flag will be set to true\n";

                                args.flags.emplace(option_name);
                            }
                            else {
                                if(option_name == "help") {
                                    std::cout << get_documentation_string(subcommand_name, default_help_verbosity, default_help_recursion_level);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    std::cerr << "Flag/option \"" << option_name << "\" (from \"" << arg_string << "\") was not recognized by " << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and is therefore being ignored\n";
                                }
                            }
                        }
                        else {
                            std::string option_or_flag_name = arg_string.substr(2, arg_string.size()-1);

                            if(subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                                if(subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                                    args.options_to_values.emplace(option_or_flag_name, std::nullopt);
                                }
                                else {
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(option_or_flag_name, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        throw std::runtime_error('\"' + arg_string + "\" referred to an option with a required argument, but no argument followed\n");
                                    }
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_or_flag_name)) {
                                args.flags.emplace(option_or_flag_name);
                            }
                            else {
                                if(option_or_flag_name == "help") {
                                    std::cout << get_documentation_string(subcommand_name, default_help_verbosity, default_help_recursion_level);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    std::cerr << command_or_subcommand << " \"" << current_subcommand_name_string << "\" does not accept a flag or option \"" << arg_string << "\". It will be ignored\n";
                                }
                            }
                        }
                    }
                }
                else if((arg_string[0] == '-') && !disambiguate_next_arg) { // short flag(s) and/or option (these are not so ez)
                    if(arg_string.find('=') != std::string::npos) {
                        for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                            std::string char_string = arg_string.substr(char_i,1);
                            if((char_i < arg_string.size()-1) && (arg_string[char_i+1] == '=')) {
                                if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, arg_string.substr(char_i+2, arg_string.size()-(char_i+2)));
                                }
                                else if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    std::cerr << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << char_string << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in flag/option group \"" << arg_string << "\"). "
                                                 "The value will be ignored and the flag will be set to true\n";

                                    args.flags.emplace(char_string);
                                }
                                else {
                                    std::cerr << "Flag/option \"" << char_string << "\" (from flag/option group \"" << arg_string << "\") was not recognized by" << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and is therefore being ignored\n";
                                }
                                break;
                            }
                            else {
                                if(subcommand_takes_flag(subcommand_name, char_string)) {
                                    args.flags.emplace(char_string);
                                }
                                else if(subcommand_takes_option(subcommand_name, char_string)) {
                                    args.options_to_values.emplace(char_string, std::nullopt);
                                }
                                else if(char_string == "h") {
                                    std::cout << get_documentation_string(subcommand_name, default_help_verbosity, default_help_recursion_level);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    std::cerr << "Flag/option \"" << char_string << "\" (from flag/option group \"" << arg_string << "\") was not recognized by" << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and is therefore being ignored\n";
                                }
                            }
                        }
                    }
                    else {
                        for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                            std::string char_string = arg_string.substr(char_i,1);

                            if(subcommand_takes_option(subcommand_name, char_string)) {
                                if(char_i < arg_string.size()-1) { // no equals sign, so everything after this character is the argument    // TODO: maybe check to see if the string contains any flags with required args, and then if not we can employ this logic
                                    args.options_to_values.emplace(char_string, arg_string.substr(char_i+1, arg_string.size()-(char_i+1)));
                                    break;
                                }
                                else if(char_i == arg_string.size()-1) {
                                    if(subcommand_option_argument_is_optional(subcommand_name, char_string)) { // if this option's argument is optional, assume that no argument is provided, and that the next arg is unrelated
                                        args.options_to_values.emplace(char_string, std::nullopt);
                                    }
                                    else {
                                        if(arg_i+1 < argc) {
                                            args.options_to_values.emplace(char_string, std::string(argv[arg_i+1]));
                                            ++arg_i; // we just ate the next arg, so don't process it again
                                        }
                                        else {
                                            throw std::runtime_error(std::string("The last character (") + arg_string[char_i] + ") in flag/option group \"" + arg_string + "\" referred to an option with a required argument, but no argument followed\n");
                                        }
                                    }
                                }
                                else {
                                    assert(false); // this should never happen
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, char_string)) {
                                args.flags.emplace(char_string);
                            }
                            else if(char_string == "h") {
                                std::cout << get_documentation_string(subcommand_name, default_help_verbosity, default_help_recursion_level);
                                return {{}, true};
                            }
                            else if(in_namespace) {
                                std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                                invalid_input_to_namespace = true;
                                continue;
                            }
                            else {
                                std::cerr << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" " // TODO: can't I make this nonfatal?
                                                         "did not form a valid flag or option for " << command_or_subcommand << " \"" << current_subcommand_name_string << "\" and will therefore be ignored\n";
                            }
                        }
                    }
                }
                else { // positional arg
                    if(arg_string == "help") {
                        std::cout << get_documentation_string(subcommand_name, default_help_verbosity, default_help_recursion_level);
                        return {{}, true};
                    }
                    else if(in_namespace) {
                        std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                        invalid_input_to_namespace = true;
                        continue;
                    }
                    else {
                        disambiguate_next_arg = false;
                        args.positional_args.emplace_back(std::move(arg_string));
                    }
                }
            }
        }
        if(is_namespace(commands.back().name)) {
            /*if(invalid_input_to_namespace) {
                std::cout << "Here is its help page:\n";
            }
            else {*/
            if(!invalid_input_to_namespace) { // TODO: make it configurable whether invalid input to namespace just gives a warning, or causes the program to stop and print help
                std::cout << '\"' << commands.back().name.back()
                          << "\" is a namespace, so using it on its own doesn't do anything. Here is its help page:\n";
                //}
                std::cout << get_documentation_string(commands.back().name, default_help_verbosity,
                                                      default_help_recursion_level);
                return {{}, true};
            }
        }

        //if(commands.size() > 0) {
            commands.back().inputs = std::move(args);
        //}

        return {std::move(commands), false};
    }
}
//end of "arg_parsing.cpp" include

//included from file "documentation.cpp"


namespace cppli {
    documentation_verbosity default_help_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
    unsigned default_help_recursion_level          = -1;

    namespace detail {
        std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs() {
            static std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t> subcommand_name_to_docs_;

            return subcommand_name_to_docs_;
        }

        std::set<std::string>& top_level_subcommands() {
            static std::set<std::string> ret;
            return ret;
        }

        bool flag_documentation_t::operator<(const flag_documentation_t& rhs) const {
            return (name < rhs.name);
        }

        flag_documentation_t::flag_documentation_t(const std::string& name, const std::string& documentation, char short_name) : name(name),
                                                                                                                                 documentation(
                                                                                                                                         documentation),
                                                                                                                                 short_name(
                                                                                                                                         short_name) {}

        bool option_documentation_t::operator<(const option_documentation_t& rhs) const {
            return std::tie(is_optional, argument_is_optional, name) <
                   std::tie(rhs.is_optional, rhs.argument_is_optional, rhs.name);
        }

        option_documentation_t::option_documentation_t(const std::string& type, const std::string& name, const std::string& argument_text,
                                                       const std::string& documentation, char short_name, bool is_optional,
                                                       bool argument_is_optional) : type(type), name(name), argument_text(argument_text),
                                                                                    documentation(documentation), short_name(short_name),
                                                                                    is_optional(is_optional),
                                                                                    argument_is_optional(argument_is_optional) {}

        positional_documentation_t::positional_documentation_t(const std::string& type, const std::string& name,
                                                               const std::string& documentation, bool optional) : type(type), name(name),
                                                                                                                  documentation(documentation),
                                                                                                                  optional(optional) {}

        variadic_documentation_t::variadic_documentation_t(const std::string& type, const std::string& name, const std::string& documentation) : type(type),
                                                                                                                                                 name(name),
                                                                                                                                                 documentation(documentation) {}


        subcommand_documentation_t::subcommand_documentation_t(const std::string& name, const char* description) : name(name), description(description), is_namespace(false) {}

        bool subcommand_documentation_t::operator<(const subcommand_documentation_t& rhs) const {
            return (name.back() < rhs.name.back());
        }

        std::string add_appropriate_brackets(const std::string& source, bool optional) {
            std::string ret = (optional ? "[" : "<");

            ret += source;

            ret += (optional ? "]" : ">");

            return ret;
        }

        struct name_and_description_t {
            std::string name,
                        description;
        };

        std::string get_documentation_string_impl(const detail::subcommand_name_t& name, documentation_verbosity verbosity, unsigned max_recursion_level, unsigned current_recursion_level, const std::optional<name_and_description_t>& main_command_override_name_and_description = std::nullopt) {

            #define FOUR_SPACES "    "
            #define EIGHT_SPACES "        "

            using namespace detail;
            const auto& docs = subcommand_name_to_docs()[name];

            std::string indent = std::string(current_recursion_level*8, ' ');
            std::string ret = indent;

            if(main_command_override_name_and_description.has_value()) {
                ret += "Options or arguments surrounded by square brackets are optional, ones surrounded by angular brackets are required.\n"
                       "Arguments of the form [arg:type...] are variadic (as indicated by the \"...\") and can receive any number of arguments (including 0)\n";
            }


            struct arg_name_and_docs_t {
                std::string name,
                            docs;

                inline arg_name_and_docs_t(const std::string& name, const std::string& docs) : name(name), docs(docs) {}
            };

            if(docs.is_namespace) {
                if(main_command_override_name_and_description.has_value()) {
                    ret += "(Main Namespace) ";
                }
                else {
                    ret += "(Namespace) ";
                }
                ret += docs.name;
                if(docs.description.size()) {
                    ret += "\n";
                    ret += indent;
                    ret += FOUR_SPACES "Description:\n";
                    ret += indent;
                    ret += EIGHT_SPACES;
                    ret += docs.description; // was  ret == docs.description;
                }
                ret += '\n';
            }
            else {
                if(main_command_override_name_and_description.has_value()) {
                    ret += "(Main Command) ";
                }
                else {
                    ret += "(Subcommand) ";
                }
                ret += docs.name;

                std::vector<arg_name_and_docs_t> positional_doc_strings;
                std::vector<arg_name_and_docs_t> flag_doc_strings;
                std::vector<arg_name_and_docs_t> option_doc_strings;

                std::string variadic_str;
                if (verbosity >= NAME_AND_ARGS) {
                    ret += ' ';

                    for (const auto& e: docs.positionals) {
                        std::string temp = add_appropriate_brackets(e.name + ":" + e.type, e.optional);
                        ret += temp;
                        ret += ' ';

                        positional_doc_strings.emplace_back(std::move(temp), e.documentation);
                    }

                    if(docs.variadic) {
                        variadic_str += "[";
                        variadic_str += docs.variadic->name;
                        variadic_str += ":";
                        variadic_str += docs.variadic->type;
                        variadic_str += "...]";

                        ret += variadic_str;
                        ret += ' ';
                    }

                    for (const auto& e: docs.flags) {
                        std::string temp = "[--";
                        temp += e.name;
                        if (e.short_name) {
                            temp += "/-";
                            temp += e.short_name;
                        }
                        temp += "]";
                        ret += temp;
                        ret += ' ';

                        flag_doc_strings.emplace_back(std::move(temp), e.documentation);
                    }

                    for (const auto& e: docs.options) {
                        std::string temp = "--";
                        temp += e.name;
                        if (e.short_name) {
                            temp += "/-";
                            temp += e.short_name;
                        }
                        temp += "=";
                        temp += add_appropriate_brackets(e.argument_text + ":" + e.type, e.argument_is_optional);

                        option_doc_strings.emplace_back(add_appropriate_brackets(temp, e.is_optional), e.documentation);

                        ret += option_doc_strings.back().name;
                        ret += ' ';
                    }
                }

                ret += '\n';

                if ((verbosity == NAME_AND_DESCRIPTION) || (verbosity > NAME_AND_ARGS)) {
                    ret += indent;
                    ret += FOUR_SPACES "Description:\n";
                    ret += indent;
                    ret += EIGHT_SPACES;
                    if(main_command_override_name_and_description.has_value()) {
                        ret += main_command_override_name_and_description->description;
                    }
                    else {
                        ret += docs.description;
                    }
                    ret += '\n';
                }

                if (verbosity == NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS) {
                    if(positional_doc_strings.size()) {
                        ret += indent;
                        ret += FOUR_SPACES "Positionals:\n";

                        for (const auto& e: positional_doc_strings) {
                            ret += indent;
                            ret += EIGHT_SPACES;
                            ret += e.name;
                            ret += ": ";
                            ret += e.docs;
                            ret += '\n';
                        }
                    }

                    if(docs.variadic) {
                        ret += indent;
                        ret += FOUR_SPACES "Variadic:\n";
                        ret += EIGHT_SPACES;
                        ret += variadic_str;
                        ret += ": ";
                        ret += docs.variadic->documentation;
                        ret += '\n';
                    }

                    if(flag_doc_strings.size()) {
                        ret += indent;
                        ret += FOUR_SPACES "Flags:\n";

                        for (const auto& e: flag_doc_strings) {
                            ret += indent;
                            ret += EIGHT_SPACES;
                            ret += e.name;
                            ret += ": ";
                            ret += e.docs;
                            ret += '\n';
                        }
                    }

                    if(option_doc_strings.size()) {
                        ret += indent;
                        ret += FOUR_SPACES "Options:\n";

                        for (const auto& e: option_doc_strings) {
                            ret += indent;
                            ret += EIGHT_SPACES;
                            ret += e.name;
                            ret += ": ";
                            ret += e.docs;

                            if(&option_doc_strings.back() == &e) {
                                ret += '\n';
                            }
                        }
                    }
                }
            }

            if(current_recursion_level+1 <= max_recursion_level) {
                if(docs.subcommands.size()) {
                    ret += indent;
                    ret += FOUR_SPACES "Subcommands:\n";

                    std::vector subcommand_name = name;
                    subcommand_name.resize(subcommand_name.size()+1);
                    for(const auto& e : docs.subcommands) {
                        subcommand_name.back() = e;

                        ret += get_documentation_string_impl(subcommand_name, verbosity, max_recursion_level, current_recursion_level + 1);


                        if((verbosity != NAME_ONLY) &&
                           (verbosity != NAME_AND_ARGS) &&
                           (&e != &*(--docs.subcommands.end()))) {
                            ret += indent;
                            ret += '\n';
                        }
                    }
                }
            }

            #undef FOUR_SPACES
            #undef EIGHT_SPACES

            return ret;
        }
    }

    std::string get_documentation_string(const detail::subcommand_name_t& name, documentation_verbosity verbosity, unsigned max_recursion_level) {
        if((name == detail::subcommand_name_t{"MAIN"}) ||
           (name == detail::subcommand_name_t{})) {
            return detail::get_documentation_string_impl({"MAIN"}, verbosity, max_recursion_level, 0, detail::name_and_description_t{detail::program_name(), detail::program_description()});
        }
        else {
            return detail::get_documentation_string_impl(name, verbosity, max_recursion_level, 0);
        }
    }

    std::string get_documentation_string(documentation_verbosity verbosity, unsigned max_recursion_level) {
        return detail::get_documentation_string_impl({"MAIN"}, verbosity, max_recursion_level, 0, detail::name_and_description_t{detail::program_name(), detail::program_description()});
    }
}
//end of "documentation.cpp" include

//included from file "subcommand.cpp"


#include <tuple>


namespace cppli::detail {

    // construct on first use idiom
    // more info here: https://isocpp.org/wiki/faq/ctors#construct-on-first-use-v2
    std::unordered_map<subcommand_name_t, subcommand_inputs_info_t, subcommand_name_hash_t>& subcommand_name_to_inputs_info() {
        static std::unordered_map<subcommand_name_t, subcommand_inputs_info_t, subcommand_name_hash_t> subcommand_name_to_inputs_info_;

        return subcommand_name_to_inputs_info_;
    }

    std::unordered_map<subcommand_name_t, subcommand_func_t, subcommand_name_hash_t>& subcommand_name_to_func() {
        static std::unordered_map<subcommand_name_t, subcommand_func_t, subcommand_name_hash_t> subcommand_name_to_func_;

        return subcommand_name_to_func_;
    }



    bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg) {
        if(parent_command_names.size()) {
            auto temp = parent_command_names;
            temp.push_back(arg);
            if(subcommand_name_to_docs().contains(temp)/* || (temp.size() == 0)*/) {
                parent_command_names = std::move(temp);
                return true;
            }
        }
        else if(subcommand_name_to_docs().at({"MAIN"}).subcommands.contains(arg)) {
            parent_command_names.push_back(arg);
            return true;
        }
        return false;
    }

    bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name) {
        return subcommand_name_to_inputs_info().contains(subcommand) &&
               subcommand_name_to_inputs_info().at(subcommand).flags.contains(flag_name);
    }

    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name) {
        return subcommand_name_to_inputs_info().contains(subcommand) &&
               subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name);
    }

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name) {
        if(subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name)) {
            return subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.at(option_name);
        }
        return false;
    }

    bool is_namespace(const subcommand_name_t& subcommand) {
        return subcommand_name_to_docs().at(subcommand).is_namespace;
    }

    bool main_command_is_namespace() {
        return true; // TODO: actual implementation
    }

    static std::string program_name_,
                       program_description_;

    const std::string& program_name() {
        return program_name_;
    }

    const std::string& program_description() {
        return program_description_;
    }

    void set_program_name_and_description(std::string&& name, std::string&& description) {
        program_name_        = std::move(name);
        program_description_ = std::move(description);
    }


    std::size_t detail::subcommand_name_hash_t::operator()(const subcommand_name_t& name) const noexcept  {
        std::size_t hash = 0;
        for(const auto& e : name) {
            cppli::detail::hash_combine(hash, e);
        }

        return hash;
    }

    bool subcommand_inputs_t::is_empty() const {
        return ((flags.size() == 0) &&
                (options_to_values.size() == 0) &&
                (positional_args.size() == 0));
    }
}
//end of "subcommand.cpp" include
#endif


#ifdef _MSC_VER
    #ifndef _MSVC_TRADITIONAL
        #error "CPPLI won't work on this version of MSVC, sorry! The version of MSVC you're using doesn't support the new preprocessor (/Zc:preprocessor)"
    #elif _MSVC_TRADITIONAL == 1
        #error "You're using the traditional preprocessor, which won't work with CPPLI. Recompile with /Zc:preprocessor"
    #endif
#endif

////

namespace cppli {
    template<detail::string_literal program_name, detail::string_literal description>
    void run(int argc, const char* const* const argv) {
        static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

        detail::set_program_name_and_description(program_name.string(), description.string());

        detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
        detail::subcommand_name_to_docs()[{"MAIN"}].description = description;

        auto parse_ret = detail::parse(argc, argv);

        if(!parse_ret.printed_help) {
            const auto& commands_vec = parse_ret.subcommands;

            bool runnable_command_found = false;
            if(!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0]);
                runnable_command_found = true;
            }

            for(unsigned i = 1; i < commands_vec.size(); ++i) {
                if((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                    runnable_command_found = true;
                    (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i]);
                }
            }

            if(!runnable_command_found) {
                std::cerr << "The input did not form any runnable commands\n";
                // TODO: print help here?
            }
        }
    }

    namespace detail {
        #define CPPLI_NAMESPACE(NAME, DESCRIPTION)

        #define CPPLI_MAIN_COMMAND(/*parameters*/...) \
        extern "C" void CPPLI_GENERATED_MAIN (__VA_ARGS__); \
        cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_subcommand<CPPLI_GENERATED_MAIN>({"MAIN"}, "")) \
        extern "C" void CPPLI_GENERATED_MAIN (__VA_ARGS__)

        #define CPPLI_SUBCOMMAND(name, DESCRIPTION, /*parameters*/...) \
        extern "C" void cppli_internal_CAT(CPPLI_GENERATED, name) (__VA_ARGS__); \
        static_assert(!::cppli::detail::contains_uppercase<cppli_internal_STRINGIFY(cppli_internal_CAT(name))>(), "subcommand names cannot contain uppercase characters"); \
        cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_subcommand<cppli_internal_CAT(CPPLI_GENERATED, name)>({cppli_internal_FOR_EACH(cppli_internal_STRINGIFY_WITH_COMMA, MAIN, name)}, DESCRIPTION)) \
        extern "C" void cppli_internal_CAT(CPPLI_GENERATED, name) (__VA_ARGS__)

        #define CPPLI_NAME(...) __VA_ARGS__

        /// the optional last argument is a single character short name
        #define CPPLI_FLAG(NAME, DESCRIPTION, /*SHORT_NAME*/...) \
        const ::cppli::detail::flag<cppli_internal_STRINGIFY(NAME), DESCRIPTION __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, bool NAME

        /// the optional last argument is a single character short name
        #define CPPLI_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
        const ::cppli::detail::option<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

        /// the optional last argument is a single character short name
        #define CPPLI_REQUIRED_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
        const ::cppli::detail::option<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME

        /// the optional last argument is a single character short name
        #define CPPLI_OPTIONAL_ARGUMENT_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
        __VA_OPT__(static_assert(isletter(__VA_ARGS__)[0]))                                               \
        const ::cppli::detail::option<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

        #define CPPLI_POSITIONAL(TYPE, NAME, DESCRIPTION) \
        const ::cppli::detail::positional<TYPE, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

        #define CPPLI_OPTIONAL_POSITIONAL(TYPE, NAME, DESCRIPTION) \
        const ::cppli::detail::positional<TYPE, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>& NAME

        #define CPPLI_VARIADIC(TYPE, NAME, DESCRIPTION) \
        const ::cppli::detail::variadic<TYPE, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME
    }
}
