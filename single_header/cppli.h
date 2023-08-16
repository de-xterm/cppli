#pragma once



//included from file "command_macros.h"


//included from file "detail/subcommand_macros.h"

namespace cppli::detail {

    /// All the recursive expansion stuff is taken from https://www.scs.stanford.edu/~dm/blog/va-opt.html
    struct evaluate_at_file_scope_dummy_t {
        template<typename T>
        evaluate_at_file_scope_dummy_t(T&&) noexcept {}
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

    #ifdef __COUNTER__
        #define cppli_internal_UNIQUE_NAME(NAME) cppli_internal_CAT(NAME, __COUNTER__) // use counter if it's available
    #else
        #define cppli_internal_UNIQUE_NAME(NAME) cppli_internal_CAT(NAME, __LINE__)  // if not, line should suffice
    #endif

    #define cppli_internal_EVALUATE_AT_FILE_SCOPE(EXPR)                        \
    namespace {                                                                \
        [[maybe_unused]]\
        const ::cppli::detail::evaluate_at_file_scope_dummy_t cppli_internal_UNIQUE_NAME(cppli_DUMMY) = EXPR; /* NOLINT */      \
    }


}
//end of "detail/subcommand_macros.h" include

//included from file "detail/command_registration.h"


//included from file "detail/subcommand.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <optional>
#include <set>
#include <cassert>
#include <iostream>


//included from file "detail/parameter_types.h"

#include <cstdint>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>


//included from file "detail/constexpr_string_literal.h"

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

        constexpr char operator[](std::size_t i) const {
            return value[i];
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

//end of "detail/constexpr_string_literal.h" include

//included from file "detail/conversions.h"

#include <string>
#include <stdexcept>
#include <limits>


//included from file "detail/user_error.h"

#include <stdexcept>
#include <variant>


//included from file "detail/template_utils.h"

#include <type_traits>

namespace cppli::detail {
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


    template<typename T, typename U, typename...Ts>
    static constexpr auto get_type_index_in_pack_func() {
        if constexpr(std::is_same_v<T, U>) {
            return 0;
        }
        else if constexpr(sizeof...(Ts) > 0) {
            return 1+get_type_index_in_pack_func<T, Ts...>();
        }
        else {
            static_assert(std::is_same_v<T, U>, "pack did not contain type");
        }
    }

    template<typename T, typename...Ts>
    static constexpr std::size_t get_type_index_in_pack_v = get_type_index_in_pack_func<T, Ts...>();
}
//end of "detail/template_utils.h" include

namespace cppli {
    namespace detail {
        using error_enum_underlying_t = uint_fast8_t;

    }
    enum minor_error_type : detail::error_enum_underlying_t {
        UNRECOGNIZED_FLAG,
        INVALID_FLAG,
        FLAG_INCLUDED_MULTIPLE_TIMES,
        FLAG_GIVEN_AN_ARGUMENT,
        OPTION_INCLUDED_MULTIPLE_TIMES,
        EXCESS_POSITIONAL,
        EXCESS_POSITIONAL_WITH_SUSPICIOUS_OPTION,
        ARGUMENT_GIVEN_TO_NAMESPACE,

        NUMBER_OF_MINOR_ERROR_TYPES
    };

    enum major_error_type : detail::error_enum_underlying_t {
        STRING_CONVERSION_ERROR,
        OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED,
        REQUIRED_OPTION_NOT_PROVIDED,
        REQUIRED_POSITIONAL_NOT_PROVIDED,

        NUMBER_OF_MAJOR_ERROR_TYPES
    };

    class user_error : public std::runtime_error {
    public:
        using error_variant_t = std::variant<minor_error_type,
                                             major_error_type>;

    private:
        error_variant_t error_variant_;

    public:
        user_error(const std::string& what, minor_error_type e);
        user_error(const std::string& what, major_error_type e);
        user_error(const std::string& what, const error_variant_t& e);

        const error_variant_t& error_type() const;
    };


    namespace detail {
        template<typename T, typename...variant_ts>
        bool is_a(const std::variant<variant_ts...>& v) {
            return v.index() ==
                   detail::get_type_index_in_pack_v<std::remove_cvref_t<T>, std::remove_cvref_t<variant_ts>...>;
        }

        //source: https://stackoverflow.com/a/62708827
        /// modified by me because it was bugged

        // A trait to check that T is one of 'Types...'
        template<typename T, typename...Types>
        struct is_one_of final : std::disjunction<std::is_same<T, Types>...> {
        };

        template<typename...Types, typename T>
        auto operator==(const std::variant<Types...>& v, T const& t) noexcept
        -> std::enable_if_t<is_one_of<T, Types...>::value, bool> {

            return is_a<T>(v) &&
                   (std::get<T>(v) == t);
        }
        // /source

        template<typename...Types, typename T>
        bool equal(const std::variant<Types...>& v, T const& t) {
            return v == t;
        }

        template<typename...Types, typename T>
        bool equal(T const& t, const std::variant<Types...>& v) {
            return v == t;
        }
    }
}
//end of "detail/user_error.h" include

namespace cppli {

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

    template<typename T>
    struct conversion_t<std::optional<T>> {
        std::optional<T> operator()(const std::optional<std::string>& s) const {
            if(s.has_value()) {
                if constexpr(std::is_constructible_v<T, const std::string&>) {
                    return std::optional<T>(std::in_place, *s);
                }
                else {
                    return std::optional<T>(conversion_t<T>()(*s));
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
                return std::optional<T>(conversion_t<T>()(s));
            }
        }

        static constexpr detail::string_literal type_string = conversion_t<T>::type_string;
    };

    template<>
    struct conversion_t<int> {
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

        static constexpr detail::string_literal type_string = "integer";
    };

    template<>
    struct conversion_t<unsigned> {
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

        static constexpr detail::string_literal type_string = "integer";
    };

    template<>
    struct conversion_t<char> {
        int operator()(const std::string& str) const {
            if(!str.size()) {
                throw user_error("Could not form a character from the given string because it was empty", STRING_CONVERSION_ERROR);
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
                throw user_error("Could not form a valid decimal from string \"" + str + "\"", STRING_CONVERSION_ERROR);
            }
            catch(std::out_of_range& e) {
                throw user_error("Could not form a valid decimal from string \"" + str + "\" because the resulting integer would be out of range", STRING_CONVERSION_ERROR);
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
//end of "detail/conversions.h" include

namespace cppli {

    namespace detail {
        // placeholder class
        template<string_literal name_, string_literal documentation_, char short_name_ = '\0'>
        class flag {
            static_assert(short_name_ == '\0' || isletter(short_name_), "flag short name must be a letter");
            static_assert(isletter(name_[0]), "flag long name must begin with a letter");
            static_assert(!contains_uppercase<name_>(), "flag long name cannot  contain uppercase characters");

        public:
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;

            flag() = default;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_ = '\0'>
        class option {
            static_assert(isletter(name_[0]), "option long name must begin with a letter");
            static_assert(!contains_uppercase<name_>(), "option long name cannot  contain uppercase characters");
            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");
            static_assert(!optional_ || std::is_constructible_v<type_, std::string> || std::is_move_constructible_v<type_>,
                          "The type parameter of an optional option must be constructible from an std::string or have a move constructor available");


        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversion_t::type_string;
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto short_name = short_name_;
            static constexpr auto documentation = documentation_;
            static constexpr auto argument_text = argument_text_;
            static constexpr auto optional = optional_;
            static constexpr auto argument_optional = argument_optional_;

            static constexpr auto has_short_name = (short_name_ != '\0');
            static constexpr auto has_long_name  = true;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                char short_name_>
        class option<type_, conversion_t_, default_construct_when_empty_, name_, documentation_, argument_text_,
                true, true,
                short_name_> {
            /// type_ if default_value == no_default_value, otherwise std::optional<type_>
        private:
            std::optional<type_> value_;
            bool was_included_ = false;

            static_assert(short_name_ == '\0' || isletter(short_name_), "option short name must be a letter");
            static_assert(isletter(name_[0]), "option long name must begin with a letter");
            static_assert(!contains_uppercase<name_>(), "option long name cannot  contain uppercase characters");

        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_.make_lowercase_and_convert_underscores();
            static constexpr auto type_string = conversion_t::type_string;
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
                    value_ = conversion_t()(*str);
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

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, bool optional_, string_literal name_, string_literal documentation_>
        class positional {
            static_assert(std::is_constructible_v<type_, std::string> || std::is_move_constructible_v<type_>,
                          "The type parameter of an optional positional argument must be constructible from an std::string or have a move constructor available");

        public:
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_;
            static constexpr auto type_string = conversion_t::type_string;
            static constexpr auto optional = true;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_>
        class positional<type_,  conversion_t_, default_construct_when_empty_, false, name_, documentation_> {
        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_;
            static constexpr auto type_string = conversion_t::type_string; // TODO: delete one of these
            static constexpr auto cppli_type_string = type_string;
            static constexpr auto optional = false;
            static constexpr auto documentation = documentation_;

            static constexpr auto has_short_name = false;
            static constexpr auto has_long_name  = false;

            positional() = default;
        };
                                                            // 99% sure this is just ignored // TODO: delete
        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_>
        class variadic {
        public:
            using type = type_;
            using conversion_t = conversion_t_;

            static constexpr auto default_construct_when_empty = default_construct_when_empty_;
            static constexpr auto name = name_;
            static constexpr auto type_string = conversion_t::type_string; // TODO: delete one of these
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

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_, string_literal argument_text_,
                bool optional_, bool argument_optional_,
                char short_name_>
        struct argument_info_t<option<type_, conversion_t_, default_construct_when_empty_, name_, documentation_, argument_text_, optional_, argument_optional_, short_name_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = true;
            static constexpr bool is_positional = false;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = true;
            static constexpr bool has_short_name = (short_name_ != '\0');
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, bool optional_, string_literal name_, string_literal documentation_>
        struct argument_info_t<positional<type_, conversion_t_, default_construct_when_empty_, optional_, name_, documentation_>> {
            static constexpr bool is_raw_type = false;
            static constexpr bool is_flag = false;
            static constexpr bool is_option = false;
            static constexpr bool is_positional = true;
            static constexpr bool is_variadic = false;

            static constexpr bool has_long_name = false;
            static constexpr bool has_short_name = false;
        };

        template<typename type_, typename conversion_t_, bool default_construct_when_empty_, string_literal name_, string_literal documentation_>
        struct argument_info_t<variadic<type_, conversion_t_, default_construct_when_empty_, name_, documentation_>> {
                static constexpr bool is_raw_type = false;
                static constexpr bool is_flag = false;
                static constexpr bool is_option = false;
                static constexpr bool is_positional = false;
                static constexpr bool is_variadic = true;

                static constexpr bool has_long_name = false;
                static constexpr bool has_short_name = false;
        };
    }
}
//end of "detail/parameter_types.h" include

namespace cppli {
    using subcommand_name_t = std::vector<std::string>;

    struct command_context_t {
        bool  current_command_is_leaf;
    };

    namespace detail {
        struct subcommand_inputs_t {
            std::vector<std::string> positional_args;
            std::unordered_map<std::string, std::optional<std::string>> options_to_values;
            std::unordered_set<std::string> flags;

            bool is_empty() const;
        };


        std::string to_string(const subcommand_name_t& name, const char* delimiter = "::");


        /// this is just boost::hash_combine, but I don't want to drag boost into this library just for one function
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

        using subcommand_func_t = void (*)(const subcommand_t&, const command_context_t& command_context);
        using subcommand_error_checking_func_t = void (*)(const subcommand_t&);

        struct subcommand_inputs_info_t {
            std::unordered_set<std::string> flags;
            std::unordered_map<std::string, bool> option_argument_is_optional;

            std::unordered_map<std::string, std::string> flag_or_option_long_name_to_short_name;
            std::unordered_map<char, std::string> flag_or_option_short_name_to_long_name;
        };

        std::unordered_map<subcommand_name_t, subcommand_inputs_info_t, subcommand_name_hash_t>&
        subcommand_name_to_inputs_info();

        std::unordered_map<subcommand_name_t, subcommand_func_t, subcommand_name_hash_t>& subcommand_name_to_func();

        std::unordered_map<subcommand_name_t, subcommand_error_checking_func_t , subcommand_name_hash_t>&
        subcommand_name_to_error_checking_func();


        /// if arg appended to parent_command_names forms a valid subcommand,
        /// pushes back arg to parent_command_names and returns true.
        /// Otherwise, just returns false
        bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg);

        bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name);

        bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name);

        bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name);

        /// string shouldn't included the leading '-' or "--"
        void error_if_flag_or_option_already_included(const subcommand_t& subcommand, const std::string& flag_or_option);

        void
        error_if_short_flag_or_option_already_included(const subcommand_t& subcommand, const std::string& flag_or_option);

        bool is_namespace(const subcommand_name_t& subcommand);

        void set_program_name_and_description(std::string&& name,
                                              std::string&& description); // todo: these should probably be in documentation.h/cpp

        const std::string& program_name();

        const std::string& program_description();

        bool main_command_is_namespace();
    }
}
//end of "detail/subcommand.h" include

//included from file "detail/documentation.h"

#include <string>


namespace cppli {
    enum documentation_verbosity {
        NAME_ONLY,
        NAME_AND_DESCRIPTION,
        NAME_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS,
        NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS
    };

    extern documentation_verbosity default_top_level_help_verbosity;
    extern documentation_verbosity default_subcommand_help_verbosity;
    extern unsigned default_help_recursion_level;
    extern bool default_hide_help_status;

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
        std::string name; // this is what we're sorting by
        std::string description;

        std::set<flag_documentation_t>          flags;    // using ordered set because we want to print commands sorted lexicographically
        std::set<option_documentation_t>        options;
        std::vector<positional_documentation_t> positionals;
        std::optional<variadic_documentation_t> variadic; // optional instead of vector because only one is allowed

        std::set<std::string>                   subcommands;

        bool is_namespace = true;

        subcommand_documentation_t() = default;
        subcommand_documentation_t(const std::string& name, const char* description);

        bool operator<(const subcommand_documentation_t& rhs) const;
    };

    using get_documentation_string_t = std::string(*)(const subcommand_name_t&,
                                                      const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                      unsigned recursion, bool hide_help);

    std::string default_get_documentation_string_callback(const subcommand_name_t&,
                                                          const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                          unsigned recursion, bool hide_help);

    extern get_documentation_string_t get_documentation_string_callback;


    /// returns documentation for the main command
    //std::string default_get_documentation_string_callback(documentation_verbosity verbosity, unsigned max_recursion_level, bool hide_help);

    namespace detail {
        std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs();

        const subcommand_documentation_t& get_command_docs_from_name(const subcommand_name_t& name);
    }
}
//end of "detail/documentation.h" include

//included from file "detail/configuration.h"

#include <string>
#include <unordered_map>


namespace cppli {
    enum error_behavior {
        SILENT,
        MESSAGE,
        THROW
    };

    namespace detail {
        void print_throw_or_do_nothing(minor_error_type error_type,
                                       const std::string& if_error_or_mesasge,
                                       const std::string& only_if_message = "");
    }

    error_behavior& minor_error_behavior(minor_error_type error_type);
}
//end of "detail/configuration.h" include

//included from file "detail/process_argument.h"


namespace cppli::detail {
    template<typename T, typename last, typename conversion_t_>
    T process_positional(unsigned cumulative_positional_index, const subcommand_t& subcommand,
                         const std::string& main_command_or_subcommand, const std::string& long_name) {
        --cumulative_positional_index; // because we've already skipped over the actual parameter for the positional, cumulative_positional_index will be too big

        if constexpr(last::optional) {
            if (cumulative_positional_index < subcommand.inputs.positional_args.size()) {
                try {
                    return conversion_t_()(
                            subcommand.inputs.positional_args[cumulative_positional_index]);
                }
                catch (user_error& e) {
                    throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                     to_string(subcommand.name) + "\" positional argument \"" +
                                     long_name + "\". Details: " + e.what(), e.error_type());
                }
            }
            else {
                return {}; // empty optional
            }
        }
        else {
            if ((cumulative_positional_index >= subcommand.inputs.positional_args.size())) {
                throw user_error(main_command_or_subcommand + " \"" + to_string(subcommand.name) +
                                 "\" required positional argument \"" + long_name +
                                 "\" was not provided (expected an argument of type " +
                                 conversion_t_::type_string.string() + ')',
                                 REQUIRED_POSITIONAL_NOT_PROVIDED);
            }
            else {
                try {
                    return conversion_t_()(
                            subcommand.inputs.positional_args[cumulative_positional_index]);
                }
                catch (user_error& e) {
                    throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                     to_string(subcommand.name) + "\" positional argument \"" +
                                     long_name + "\". Details: " + e.what(), e.error_type());
                }
            }
        }
    }

    template<typename T, typename last, typename conversion_t_>
    T process_variadic(unsigned cumulative_positional_index, const subcommand_t& subcommand,
                       const std::string& main_command_or_subcommand, const std::string& long_name) {
        T ret; // T is a vector
        for (unsigned i = cumulative_positional_index;
             i < subcommand.inputs.positional_args.size(); ++i) {
            try {
                ret.emplace_back(conversion_t_()(subcommand.inputs.positional_args[i]));
            }
            catch (user_error& e) {
                throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                 to_string(subcommand.name) + "\" variadic argument pack \"" +
                                 long_name + "\" at argument index " + std::to_string(i) +
                                 ". Details: " + e.what(), e.error_type());
            }
        }
        return ret;
    }

    template<typename T, typename last, typename conversion_t_>
    T process_required_option(const subcommand_t& subcommand,
                                       const std::string& main_command_or_subcommand,
                                       const std::string& long_name, const std::optional<std::string>& short_name) {
        
        if (!subcommand.inputs.options_to_values.contains(long_name) &&
            (short_name && !subcommand.inputs.options_to_values.contains(*short_name))) {

            throw user_error(main_command_or_subcommand + " \"" + to_string(subcommand.name) +
                             "\" was not provided with required option \"" + long_name + '"',
                             REQUIRED_OPTION_NOT_PROVIDED);
        }
        else if ((subcommand.inputs.options_to_values.contains(long_name) &&
                  !subcommand.inputs.options_to_values.at(long_name).has_value()) ||
                 (short_name && subcommand.inputs.options_to_values.contains(*short_name) &&
                  !subcommand.inputs.options_to_values.at(*short_name).has_value())) {

            throw user_error(
                    main_command_or_subcommand + " \"" + to_string(subcommand.name) + "\" option \""
                    + long_name + "\" requires an argument, but one was not provided "
                                       "(expected an argument of type "
                    + conversion_t_::type_string.make_lowercase_and_convert_underscores().string() +
                    ')',
                    OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
        }
        else { // by this point, none of the optionals we're interested in are empty
            if (subcommand.inputs.options_to_values.contains(long_name)) {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(long_name));
                }
                catch (user_error& e) {
                    throw user_error("Error initializing " + main_command_or_subcommand + " \""
                                     + to_string(subcommand.name) + "\" option \"" +
                                     long_name + "\". Details: " + e.what(), e.error_type());
                }
            }
            else { // has_short_name is guaranteed to be true at this point
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(*short_name));
                }
                catch (user_error& e) {
                    throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                     to_string(subcommand.name) + "\" option \"" + *short_name +
                                     "\" (full name \"" + long_name + "\"). Details: " +
                                     e.what(), e.error_type());
                }
            }
        }
    }

    template<typename T, typename last, typename conversion_t_>
    T process_required_argument_option(const subcommand_t& subcommand,
                                       const std::string& main_command_or_subcommand,
                                       const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            if (!subcommand.inputs.options_to_values.at(
                    long_name).has_value()) { // TODO: aren't we doing this check in arg_parsing.cpp too?
                throw user_error(main_command_or_subcommand + to_string(subcommand.name) +
                                 "\" option \"" + long_name + "\" "
                                                                   "requires an argument, but one was not provided (expected an argument of type "
                                 +
                                 static_cast<std::string>(conversion_t_::type_string.make_lowercase_and_convert_underscores()) +
                                 "."
                                 "Note that this option is optional, so it is valid to omit it entirely, "
                                 "but the option's argument is required, so if the option is provided, it must come with an argument",
                                 OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
            }
            else {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(
                            long_name)); // no need for has_value check here; returning an empty optional is valid
                }
                catch (user_error& e) {
                    throw user_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + long_name +
                            "\". Details: " + e.what(), e.error_type());
                }
            }
        }
        else if (short_name && subcommand.inputs.options_to_values.contains(
                *short_name)) { // TODO: evaluating short_name could use if constexpr
            if (!subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                throw user_error(main_command_or_subcommand + to_string(subcommand.name) +
                                 "\" option \"" + *short_name + "\" (full name \"" +
                                 long_name + "\") "
                                                  "requires an argument, but one was not provided (expected an argument of type "
                                 + conversion_t_::type_string.string() + "."
                                                                        "Note that this option is optional, so it is valid to omit it entirely, "
                                                                        "but the option's argument is required, so if the option is provided, it must come with an argument",
                                 OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
            }
            else {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(
                            *short_name)); // no need for has_value check here; returning an empty optional is valid
                }
                catch (user_error& e) {
                    throw user_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + *short_name +
                            "\" (full name \"" + long_name + "\"). Details: " + e.what(),
                            e.error_type());
                }
            }
        }
        else {
            return {};
        }
    }

    template<typename T, typename last, typename conversion_t_>
    T process_raw_type_optional_argument_option(const subcommand_t& subcommand,
                                                const std::string& main_command_or_subcommand,
                                                const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            if (subcommand.inputs.options_to_values.at(long_name).has_value()) {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(
                            long_name)); // no need for has_value check here; returning an empty optional is valid
                }
                catch (user_error& e) {
                    throw user_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + long_name +
                            "\". Details: " + e.what(), e.error_type());
                }
            }
            else {
                return typename last::type();
            }
        }
        else if (short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
            if (subcommand.inputs.options_to_values.at(*short_name).has_value()) {
                try {
                    return conversion_t_()(*subcommand.inputs.options_to_values.at(*short_name));
                }
                catch (user_error& e) {
                    throw user_error(
                            "Error initializing " + main_command_or_subcommand + " \"" +
                            to_string(subcommand.name) + "\" option \"" + *short_name +
                            "\" (full name \"" + long_name + "\"). Details: " +
                            e.what(), e.error_type());
                }
            }
            else {
                return typename last::type();
            }
        }
        else {
            return {}; // return empty optional
        }
    }

    template<typename T, typename last>
    T process_optional_argument_option(const subcommand_t& subcommand,
                                                const std::string& main_command_or_subcommand,
                                                const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            try {
                return {subcommand.inputs.options_to_values.at(
                        long_name)}; // no need for has_value check here; returning an empty optional is valid
            }
            catch (user_error& e) {
                throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                 to_string(subcommand.name) + "\" option \"" + long_name +
                                 "\". Details: " + e.what(), e.error_type());
            }
        }
        else if (short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
            try {
                return {subcommand.inputs.options_to_values.at(
                        *short_name)}; // no need for has_value check here; returning an empty optional is valid
            }
            catch (user_error& e) {
                throw user_error("Error initializing " + main_command_or_subcommand + " \"" +
                                 to_string(subcommand.name) + "\" option \"" + *short_name +
                                 "\" (full name \"" + long_name + "\"). Details: " + e.what(),
                                 e.error_type());
            }
        }
        else {
            return {}; // default constructor for optional option with optional argument inits to empty optional and was_included to false
        }
    }

    template<typename T, typename last>
    T process_raw_type(unsigned cumulative_positional_index, const subcommand_t& subcommand,
                                       const std::string& main_command_or_subcommand) {

        std::string long_name = last::name.string();

        std::optional<std::string> short_name;

        if constexpr(last::has_short_name) {
            short_name = std::string{last::short_name};
        }

        if constexpr(argument_info_t<last>::is_flag) {
            return subcommand.inputs.flags.contains(last::name.string()) ||
                   (short_name && subcommand.inputs.flags.contains(std::string{last::short_name}));
        }
        else {
            using conversion_t_ = typename last::conversion_t;

            if constexpr(argument_info_t<last>::is_option) {
                if constexpr(last::optional) { // implicitly required argument
                    if constexpr(last::argument_optional) { // I believe default_construct_when_empty is implicity true, so no need for && here
                        return process_raw_type_optional_argument_option<T, last, conversion_t_>(subcommand, main_command_or_subcommand,
                                                                                  long_name, short_name);
                    }
                    else {
                        return process_required_argument_option<T, last, conversion_t_>(subcommand, main_command_or_subcommand, 
                                                                                       long_name, short_name);
                    }
                }
                else {
                    return process_required_option<T, last, conversion_t_>(subcommand,
                                                                          main_command_or_subcommand,
                                                                          long_name, short_name);
                }
            }
            else if constexpr(argument_info_t<last>::is_positional) { // positional
                return process_positional<T, last, conversion_t_>(cumulative_positional_index, subcommand,
                                                                 main_command_or_subcommand, long_name);
            }
            else { // variadic
                return process_variadic<T, last, conversion_t_>(cumulative_positional_index, subcommand,
                                                               main_command_or_subcommand, long_name);
            }
        }
    }

    template<typename T, typename last>
    T process_wrapper(const subcommand_t& subcommand,
                      const std::string& main_command_or_subcommand) {


        std::string long_name = T::name.string();

        std::optional<std::string> short_name;

        if constexpr(T::has_short_name) {
            short_name = std::string{T::short_name};
        }

        if constexpr(argument_info_t<T>::is_option) {
            if constexpr(T::optional && T::argument_optional) {
                return process_optional_argument_option<T, last>(subcommand,
                                                                 main_command_or_subcommand,
                                                                 long_name, short_name);
            }
            else {
                return {}; // this variable is a dummy
            }
        }
        else {
            return {}; // dummy
        }
    }

    template<typename T, typename last>
    /*add_const_ref_if_string_t<T>::*/T
    process_argument(unsigned cumulative_positional_index, const subcommand_t& subcommand) {
        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument

        std::string main_command_or_subcommand;

        if(subcommand.name == subcommand_name_t{"MAIN"}) {
            main_command_or_subcommand = "main command";
        }
        else {
            main_command_or_subcommand = "subcommand";
        }

        if constexpr(arg_info_t::is_raw_type) {
            return process_raw_type<T, last>(cumulative_positional_index, subcommand,
                                                           main_command_or_subcommand);
        }
        else {
            return process_wrapper<T, last>(subcommand, main_command_or_subcommand);
        }
    }
}

//end of "detail/process_argument.h" include

//included from file "detail/argument_processing_checks.h"

namespace cppli::detail {
    template<std::size_t current_index, std::size_t max_index, typename arg_t, typename...arg_ts>
    constexpr std::size_t count_positionals_before_index_func() {
        if constexpr(current_index < max_index) {
            return count_positionals_before_index_func<current_index+1, max_index, arg_ts...>()+argument_info_t<std::remove_cvref_t<arg_t>>::is_positional;
        }
        else {
            return 0; // return 0 instead of is_positional because inclusivity is not what we want
        }
    }

    struct dummy_t{};

    // TODO: static_assert for optional positions followed by required positionals
    template<bool optional_positional_encountered, typename arg_t, typename...arg_ts>
    constexpr bool invalid_optional_positionals_found() {
        using T = std::remove_cvref_t<arg_t>;
        constexpr bool is_positional = argument_info_t<T>::is_positional;

        if constexpr(sizeof...(arg_ts) > 0) {
            if constexpr(is_positional) {
                return (optional_positional_encountered && !T::optional) || invalid_optional_positionals_found<optional_positional_encountered || T::optional, arg_ts...>();
            }
            else {
                return invalid_optional_positionals_found<optional_positional_encountered, arg_ts...>();
            }
        }
        else {
            if constexpr(is_positional) {
                return optional_positional_encountered && !T::optional;
            }
            else {
                return false;
            }
        }
    }

    template<typename arg_t, typename...arg_ts>
    constexpr std::size_t count_variadics() {
        if constexpr(sizeof...(arg_ts) > 0) {
            return argument_info_t<arg_t>::is_variadic+count_variadics<arg_ts...>();
        }
        else {
            return 0;
        }
    }

    template<std::size_t max_index, typename...arg_ts>
    static constexpr std::size_t count_positionals_before_index_v = count_positionals_before_index_func<0, max_index, arg_ts...>();

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

    template<std::size_t index, typename first_t, typename...rest_ts> // this is broken
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
}
//end of "detail/argument_processing_checks.h" include

namespace cppli::detail {
    template<typename func_t, auto func, typename index_seq>
    struct call_func_wrapper_impl_t;

    template<typename return_t, typename...arg_ts, std::size_t...indices, auto func>
    struct call_func_wrapper_impl_t<return_t(*)(const command_context_t&, arg_ts...), func, std::integer_sequence<std::size_t, indices...>> {
        static void call_func(const subcommand_t& subcommand, const command_context_t& command_context) {
            func(command_context, process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>, std::remove_cvref_t<get_type_from_index_in_pack<(indices)-1, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand)...);
        }

        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            static void check_for_errors(const subcommand_t& subcommand) {
                // fold over comma operator
                (process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>, std::remove_cvref_t<get_type_from_index_in_pack<(indices)-1, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand), ...);
            }
        #endif
    };

    template<typename func_t, auto func>
    struct call_func_wrapper_t;

    template<typename return_t, typename...arg_ts, auto func>
    struct call_func_wrapper_t<return_t(*)(const command_context_t&, arg_ts...), func> { // we need all these ugly partial specializations so that we can deduce arg_ts and indices
        static_assert(!invalid_optional_positionals_found<false, arg_ts...>(), "Required positionals cannot follow optional positionals");

        static void call_func(const subcommand_t& subcommand, const command_context_t& command_context) {
            constexpr auto variadic_count = count_variadics<std::remove_cvref_t<arg_ts>...>();
            static_assert(variadic_count < 2, "A command can only have 0 or 1 variadics (two or more were found)");

            call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::call_func(subcommand, command_context);
        }

        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            static void check_for_errors(const subcommand_t& subcommand) {
                call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::check_for_errors(subcommand);
            }
        #endif
    };

    template<auto func>
    struct call_func_wrapper_t<void(*)(const command_context_t&), func> { // we need all this ugly partial specializations so that we can deduce arg_ts and indices
        static void call_func(const subcommand_t& subcommand, const command_context_t& command_context) {
            func(command_context);
        }

        static void check_for_errors(const subcommand_t& subcommand) {
            // do nothing
        }
    };

    template<auto func>
    void call_func(const subcommand_t& command, const command_context_t& command_context) {
        call_func_wrapper_t<decltype(func), func>::call_func(command, command_context);
    }

    #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
        template<auto func>
        void check_for_errors(const subcommand_t& command) {
            call_func_wrapper_t<decltype(func), func>::check_for_errors(command);
        }
    #endif


    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(arg_t, arg_ts...) = nullptr) {
        using T = std::remove_cvref_t<arg_t>;
        using arg_info_t = argument_info_t<T>;

        if constexpr(!arg_info_t::is_raw_type) {
            using type = std::remove_cvref_t<arg_t>;

            static_assert(no_repeated_short_names_v<arg_t, arg_ts...>, "multiple flags/options cannot share a short name");
            static_assert(no_repeated_long_names_v<arg_t, arg_ts...>,  "multiple flags/options cannot share a long name"); // I don't think this is necessary anymore

            if constexpr(arg_info_t::is_flag) {
                documentation.flags.emplace(type::name.string(),
                                            type::documentation.string(),
                                            type::short_name);

                info.flags.insert(type::name.string());

                if constexpr(type::short_name != '\0') {
                    info.flags.insert(std::string{type::short_name});

                    info.flag_or_option_short_name_to_long_name.emplace(T::short_name, T::name.string());
                    info.flag_or_option_long_name_to_short_name.emplace(T::name.string(),  std::string{T::short_name});
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

                    info.flag_or_option_short_name_to_long_name.emplace(T::short_name, T::name.string());
                    info.flag_or_option_long_name_to_short_name.emplace(T::name.string(),  std::string{T::short_name});
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


    // we don't have command_macros yet, so we have to write the callback signature manually
    void default_help_callback(const command_context_t& cppli_current_command,

                               const variadic<std::string, conversion_t<std::string>, false, "subcommand name", "The name of the subcommand to print help for."
                                                                                                                "If no subcommand is provided, then help is printed for the parent command">&, std::vector<std::string> subcommand_name,

                               const flag<"name-only", "only print subcommand names">&,                                  bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&,             bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&,                           bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print subcommand name and description", 'v'>&,                     bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&,                    bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&,                       bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&,                                  bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&,             bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&,                           bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&,                          bool subcommands_verbose,

                               const option<unsigned, conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion);

    template<auto func>
    dummy_t register_command(const subcommand_name_t& name, const char* description, bool is_help = false) {
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

        subcommand_name_to_func().insert_or_assign(name, call_func<func>);
        #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
            subcommand_name_to_error_checking_func().insert_or_assign(name, check_for_errors<func>);
        #endif
        subcommand_name_to_inputs_info().insert_or_assign(name, std::move(info));

        if(name == subcommand_name_t{"MAIN"}) {
            subcommand_name_to_docs()[name].flags       = std::move(docs.flags);
            subcommand_name_to_docs()[name].options     = std::move(docs.options);
            subcommand_name_to_docs()[name].positionals = std::move(docs.positionals);
            subcommand_name_to_docs()[name].variadic    = std::move(docs.variadic);

            subcommand_name_to_docs()[name].is_namespace = false;

            // can't do a full assignment because that would wipe subcommands
        }
        else {
            subcommand_name_to_docs().insert_or_assign(name, std::move(docs));
        }

        if(!is_help) {
            subcommand_name_t temp = name;
            temp.push_back("help");

            if(!subcommand_name_to_func().contains(temp)) {
                register_command<default_help_callback>(temp, "print help for this command", true);
            }
        }

        return {};
    }
}

//end of "detail/command_registration.h" include

#ifdef _MSC_VER
    #ifndef _MSVC_TRADITIONAL
        #error "CPPLI won't work on this version of MSVC, sorry! The version of MSVC you're using doesn't support the new preprocessor (/Zc:preprocessor)"
    #elif _MSVC_TRADITIONAL == 1
        #error "You're using the traditional preprocessor, which won't work with CPPLI. Recompile with /Zc:preprocessor"
    #endif
#endif

namespace cppli {
    bool current_command_is_leaf();

    //#define CPPLI_NAMESPACE(NAME, DESCRIPTION)

    #define CPPLI_MAIN_COMMAND(/*parameters*/...) \
    extern "C" void CPPLI_INTERNAL_GENERATED_MAIN_COMMAND_CALLBACK (const command_context_t& __VA_OPT__(, __VA_ARGS__)); \
    cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_command<CPPLI_INTERNAL_GENERATED_MAIN_COMMAND_CALLBACK>({"MAIN"}, "")) \
    extern "C" void CPPLI_INTERNAL_GENERATED_MAIN_COMMAND_CALLBACK (const cppli::command_context_t& cppli __VA_OPT__(, __VA_ARGS__))

    #define CPPLI_SUBCOMMAND(name, DESCRIPTION, /*parameters*/...) \
    extern "C" void cppli_internal_CAT(CPPLI_INTERNAL_GENERATED_SUBCOMMAND_CALLBACK, name) (const cppli::command_context_t& __VA_OPT__(, __VA_ARGS__)); \
    static_assert(!::cppli::detail::contains_uppercase<cppli_internal_STRINGIFY(cppli_internal_CAT(name))>(), "subcommand names cannot contain uppercase characters"); \
    cppli_internal_EVALUATE_AT_FILE_SCOPE(::cppli::detail::register_command<cppli_internal_CAT(CPPLI_INTERNAL_GENERATED_SUBCOMMAND_CALLBACK, name)>({cppli_internal_FOR_EACH(cppli_internal_STRINGIFY_WITH_COMMA, MAIN, name)}, DESCRIPTION)) \
    extern "C" void cppli_internal_CAT(CPPLI_INTERNAL_GENERATED_SUBCOMMAND_CALLBACK, name) (const cppli::command_context_t& cppli __VA_OPT__(, __VA_ARGS__))

    #define CPPLI_NAME(...) __VA_ARGS__

    /// the optional last argument is a single character short name
    #define CPPLI_FLAG(NAME, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::flag<cppli_internal_STRINGIFY(NAME), DESCRIPTION __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, bool NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversion_t<std::optional<TYPE>>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const std::optional<TYPE>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const std::optional<TYPE>& NAME

    /// same as CPPLI_OPTION, but the raw TYPE is used (without std::optional)
    /// If the option is not provided, a default constructed object is passed to the callback instead of an empty optional
    /// The optional last argument is a single character short name
    #define CPPLI_OPTION_DEFAULT_CTOR(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversion_t<TYPE>, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const TYPE& NAME

    #define CPPLI_OPTION_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>, const TYPE& NAME


    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversion_t<std::optional<TYPE>>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_DEFAULT_CTOR(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversion_t<std::optional<TYPE>>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_OPTIONAL_ARGUMENT_OPTION_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, true, true __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const std::optional<TYPE>& NAME



    /// the optional last argument is a single character short name
    #define CPPLI_REQUIRED_OPTION(TYPE, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, ::cppli::conversion_t<TYPE>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME

    /// the optional last argument is a single character short name
    #define CPPLI_REQUIRED_OPTION_CONVERSION(TYPE, CONVERSION_T, NAME, ARGUMENT_TEXT, DESCRIPTION, /*SHORT_NAME*/...) \
    const ::cppli::detail::option<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION, ARGUMENT_TEXT, false, false __VA_OPT__(, cppli_internal_STRINGIFY(__VA_ARGS__)[0])>&, const TYPE& NAME


    #define CPPLI_POSITIONAL(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::conversion_t<TYPE>, false, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

    #define CPPLI_POSITIONAL_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME


    #define CPPLI_OPTIONAL_POSITIONAL(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::conversion_t<std::optional<TYPE>>, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::optional<TYPE>& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::optional<TYPE>& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_DEFAULT_CTOR(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, ::cppli::conversion_t<TYPE>, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME

    #define CPPLI_OPTIONAL_POSITIONAL_CONVERSION_DEFAULT_CTOR(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::positional<TYPE, CONVERSION_T, false, true, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const TYPE& NAME


    #define CPPLI_VARIADIC(TYPE, NAME, DESCRIPTION) \
    const ::cppli::detail::variadic<TYPE, ::cppli::conversion_t<TYPE>, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME

    #define CPPLI_VARIADIC_CONVERSION(TYPE, CONVERSION_T, NAME, DESCRIPTION) \
    const ::cppli::detail::variadic<TYPE, CONVERSION_T, false, cppli_internal_STRINGIFY(NAME), DESCRIPTION>&, const std::vector<TYPE>& NAME
}
//end of "command_macros.h" include

//included from file "configuration.h"


//end of "configuration.h" include

//included from file "documentation.h"



//end of "documentation.h" include

//included from file "run.h"


namespace cppli {

    namespace detail {
        void run_impl_(int argc, const char* const* const argv);
    }

    template<detail::string_literal program_name, detail::string_literal description> // TODO: put run in its own file
    void run(int argc, const char* const* const argv) {
        static_assert(detail::all_lowercase_numeral_or_hyphen<program_name>(), "command names can only contain lowercase characters, numerals, and hyphens");

        detail::set_program_name_and_description(program_name.string(), description.string());

        detail::subcommand_name_to_docs()[{"MAIN"}].name = program_name;
        detail::subcommand_name_to_docs()[{"MAIN"}].description = description;

        detail::run_impl_(argc, argv);
    }
}
//end of "run.h" include

#ifdef CPPLI_IMPLEMENTATION

//included from file "configuration.cpp"

#include <iostream>


namespace cppli {
    namespace detail {
        void print_throw_or_do_nothing(minor_error_type error_type,
                                       const std::string& print_if_error_or_mesasge,
                                       const std::string& print_only_if_message) {
            if(minor_error_behavior(error_type) == THROW) {
                throw user_error(print_if_error_or_mesasge, error_type);
            }
            else {
                std::cerr << print_if_error_or_mesasge << print_only_if_message;
            }
        }
    }

    error_behavior& minor_error_behavior(minor_error_type error_type) {
        static std::unordered_map<minor_error_type, error_behavior> minor_error_code_to_behavior;

        if(!minor_error_code_to_behavior.size()) {
            for(unsigned i = 0; i < NUMBER_OF_MINOR_ERROR_TYPES; ++i) {
                minor_error_code_to_behavior.emplace(static_cast<minor_error_type>(i), MESSAGE);
            }
        }

        return minor_error_code_to_behavior.at(error_type);
    }
}
//end of "configuration.cpp" include

//included from file "arg_parsing.cpp"

#include <cassert>
#include <iostream>
#include <sstream>


//included from file "detail/arg_parsing.h"


namespace cppli::detail {
    struct parse_ret_t {
        std::vector<subcommand_t> subcommands;
        bool printed_help = false;
        std::optional<unsigned> help_command_index;
    };

    parse_ret_t parse(int argc, const char* const* const argv);
}
//end of "detail/arg_parsing.h" include

namespace cppli::detail {
    bool contains_letters(const std::string& str) {
        bool contains = false;
        for(char c : str) {
            contains |= isletter(c);
        }

        return contains;
    }

    std::string say_something_if_empty(const std::string& arg) {
        if(!arg.size()) {
            return "(empty string)";
        }
        else {
            return '\"' + arg + '\"';
        }
    }

    // TODO: break each major if in the loop into a function, and put each function in its own file
    parse_ret_t parse(int argc, const char* const* const argv) {

        if(argc == 0) {
            std::cerr << "Error: argc == 0. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }
        if(!argv[0]) {
            std::cerr << "Error: argv[0] was null. This is very terrible and unrecoverable\n";
            std::exit(-1);
        }

        /*if(program_name() != argv[0]) { // TODO: switch to enable/disable this warning. I'm commenting it out for now because it's annoying
            std::cerr << "The name used to invoke the program (\"" << argv[0] << "\") was not the same as the program's documented name (\"" << program_name() << "\")\n";
        }*/

        // skip the program name
        subcommand_name_t subcommand_name = {"MAIN"};

        //subcommand_inputs_t args;
        std::vector<subcommand_t> commands{{subcommand_name}};

        bool disambiguate_next_arg = false;
        bool disambiguate_until_subcommand = false;
        bool disambiguate_all = false;

        bool invalid_input_to_namespace = false;
                                                    // possibly the longest variable name I've ever written
        std::unordered_map<unsigned, std::string> optional_argument_option_with_no_value_provided_arg_index_to_option_string; // indices in argv of optional argument optionals that weren't provided an argument
                                                                                                                              // used to make better error messages
        std::string first_command_name = argv[0];

        bool in_namespace = is_namespace({"MAIN"});

        std::optional<unsigned> help_command_index;

        std::string command_or_subcommand = "main command";

        std::string current_subcommand_name_string = to_string(subcommand_name);

        parse_ret_t ret;

        for(unsigned arg_i = 1; arg_i < argc; ++arg_i) {
            std::string arg_string = argv[arg_i];

            if(is_valid_subcommand(subcommand_name, arg_string) && !(disambiguate_next_arg || disambiguate_all)) {
                if(arg_string == "help") {
                    ret.help_command_index = commands.size();
                }
                in_namespace = is_namespace(subcommand_name);

                command_or_subcommand = "subcommand";
                current_subcommand_name_string = to_string(subcommand_name);

                disambiguate_until_subcommand = false;

                //commands.back().inputs = std::move(args);
                //args = {};

                commands.push_back({subcommand_name});
            }
            else {
                subcommand_inputs_t& args = commands.back().inputs;
                /*if(in_namespace) { // I forgot why this doesn't work
                    std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                    continue;
                }*/
                if((arg_string.substr(0,2) == "--") && !(disambiguate_next_arg || disambiguate_until_subcommand || disambiguate_all)) { // long flag (these are ez)
                                                    // we need this check so that we can handle the case where "--" is the thing we're trying to disambiguate. Ex: "program -- --"
                    if(arg_string[2] == '1') { // arg_string == "--1"
                        disambiguate_next_arg = true;
                    }
                    else if(arg_string.size() == 2) { // arg_string == "--"
                        disambiguate_until_subcommand = true;
                    }
                    else if(arg_string[2] == '-') { // arg_string == "---"
                        disambiguate_all = true;
                    }
                    else {
                        std::string::size_type equals_pos;
                        if((equals_pos = arg_string.find('=')) != std::string::npos) { // TODO: make sure the -2 doesn't cause issues
                            std::string option_name  = arg_string.substr(2, equals_pos-2),
                                        option_value = arg_string.substr(equals_pos+1, arg_string.size()-(equals_pos+1));

                            if(subcommand_takes_option(subcommand_name, option_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_name);
                                args.options_to_values.emplace(option_name, option_value);
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_name)) {
                                std::stringstream ss;
                                ss << "For " << command_or_subcommand << " \"" << current_subcommand_name_string << "\", \"" << option_name << "\" is a flag, not an option, and therefore can't be assigned a value (like it was in \"" << arg_string << "\"). ";

                                print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(), "The value will be ignored and the flag will be set to true\n\n");

                                args.flags.emplace(option_name);
                            }
                            else {
                                if(option_name == "help") {
                                    std::cout << get_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                                    return {{}, true};
                                }
                                else if(in_namespace) {
                                    std::stringstream ss;
                                    ss << '\"' << current_subcommand_name_string << "\" is a namespace, so it cannot accept the input \"" << arg_string << "\" (the only inputs it can accept are --help, -h, and help)";

                                    print_throw_or_do_nothing(ARGUMENT_GIVEN_TO_NAMESPACE, ss.str(), "The given input be ignored\n\n");
                                    invalid_input_to_namespace = true;
                                    continue;
                                }
                                else {
                                    std::stringstream ss;
                                    ss << "Flag/option \"" << option_name << "\" (from \"" << arg_string << "\") was not recognized by " << command_or_subcommand << " \"" << current_subcommand_name_string << "\"\n";

                                    print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), "It will be ignored\n\n");
                                }
                            }
                        }
                        else {
                            std::string option_or_flag_name = arg_string.substr(2, arg_string.size()-1);

                            //error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                            if(subcommand_takes_option(subcommand_name, option_or_flag_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);

                                if(subcommand_option_argument_is_optional(subcommand_name, option_or_flag_name)) {
                                    args.options_to_values.emplace(option_or_flag_name, std::nullopt);
                                    optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i, arg_string);
                                }
                                else {
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(option_or_flag_name, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        std::stringstream ss;
                                        ss << "Argument \"" << arg_string
                                           << "\" given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                                           << " referred to an option with a required argument, but no argument followed\n";

                                        throw user_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                    }
                                }
                            }
                            else if(subcommand_takes_flag(subcommand_name, option_or_flag_name)) {
                                error_if_flag_or_option_already_included(commands.back(), option_or_flag_name);
                                args.flags.emplace(option_or_flag_name);
                            }
                            else {
                                if(option_or_flag_name == "help") {
                                    std::cout << get_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
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
                }                                                           // so that string like "-" and " - " can be used as positionals without issue
                else if((arg_string[0] == '-') && !(disambiguate_next_arg || disambiguate_until_subcommand || disambiguate_all)) { // short flag(s) and/or option (these are not so ez)
                    bool invalid_character_in_flag_group = false;
                    unsigned invalid_character_index;
                    std::stringstream invalid_character_in_flag_group_message;

                    for(unsigned char_i = 1; char_i < arg_string.size(); ++char_i) {
                        std::string char_string = arg_string.substr(char_i,1);

                        if(subcommand_takes_option(subcommand_name, char_string)) { // there is an argument
                            error_if_short_flag_or_option_already_included(commands.back(), char_string);
                            if(char_i < arg_string.size()-1) { // no equals sign, so everything after this character is the argument    // TODO: maybe check to see if the string contains any flags with required args, and then if not we can employ this logic
                                args.options_to_values.emplace(char_string, arg_string.substr(char_i+1+(arg_string[char_i+1] == '='), arg_string.size()));
                                break;                                                                 // ^ discard a leading '='!!
                            }
                            else if(char_i == arg_string.size()-1) { // no room for argument
                                if(subcommand_option_argument_is_optional(subcommand_name, char_string)) { // if this option's argument is optional, assume that no argument is provided, and that the next arg is unrelated
                                    args.options_to_values.emplace(char_string, std::nullopt);
                                    optional_argument_option_with_no_value_provided_arg_index_to_option_string.emplace(arg_i, "-" + char_string);
                                }
                                else {
                                    if(arg_i+1 < argc) {
                                        args.options_to_values.emplace(char_string, std::string(argv[arg_i+1]));
                                        ++arg_i; // we just ate the next arg, so don't process it again
                                    }
                                    else {
                                        std::stringstream ss;
                                        ss << "The last character '" << arg_string[char_i]
                                           << "' in flag/option group \"" << arg_string << "\" "
                                              "given to " << command_or_subcommand << ' ' << current_subcommand_name_string
                                           << " referred to an option with a required argument, but no argument followed\n";

                                        throw user_error(ss.str(), OPTION_REQUIRED_ARGUMENT_NOT_PROVIDED);
                                    }
                                }
                            }
                            else {
                                assert(false); // this should never happen (would mean char_i is out of range of arg_string)
                            }
                        }
                        else if(subcommand_takes_flag(subcommand_name, char_string)) {
                            error_if_short_flag_or_option_already_included(commands.back(), char_string);
                            args.flags.emplace(char_string);
                        }
                        else if(char_string == "h") {
                            std::cout << get_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                            return {{}, true};
                        }
                        else if(in_namespace) {
                            std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                            invalid_input_to_namespace = true;
                            continue;
                        }
                        else {
                            if(!isletter(char_string[0])) {
                                if(!invalid_character_in_flag_group) {
                                    invalid_character_index = char_i; // because we only want to set this the first time we find an invalid character
                                }
                                invalid_character_in_flag_group = true;
                                invalid_character_in_flag_group_message << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" "
                                   << "given to " << command_or_subcommand << " \"" << current_subcommand_name_string << "\" "
                                   << "is not a letter and therefore cannot form a valid short name for a flag or option\n";

                                if(arg_string[invalid_character_index] == '=') {
                                    break; // so that we don't continue to look for flags (valid or invalid) in the "argument"
                                }
                            }
                            else {
                                std::stringstream ss;
                                ss << "Character '" << char_string << "' in flag/option group \"" << arg_string << "\" "
                                      "was not a recognized flag or option for " << command_or_subcommand << " \"" << current_subcommand_name_string << "\".";

                                print_throw_or_do_nothing(UNRECOGNIZED_FLAG, ss.str(), " It will be ignored\n\n");
                            }
                        }
                    }

                    if(invalid_character_in_flag_group) {
                        if(invalid_character_index > 1) { // 1 instead of 0 because of the leading '-'
                            if(arg_string[invalid_character_index] == '=') {
                                std::stringstream ss;
                                ss << "Character '" << arg_string[invalid_character_index-1] << "' in flag/option group \"" << arg_string << "\" is a flag, and therefore can't accept an argument.\n";

                                print_throw_or_do_nothing(FLAG_GIVEN_AN_ARGUMENT, ss.str(),
                                                          "The argument " + say_something_if_empty(arg_string.substr(invalid_character_index+1, arg_string.size())) +
                                                          " will be ignored and the flag '" + arg_string[invalid_character_index-1] + "' will be set to true\n");
                                continue;
                            }
                            else {
                                invalid_character_in_flag_group_message << "It's possible that the substring \"" << arg_string.substr(invalid_character_index, arg_string.size())
                                                                        << "\" was meant to be an argument for '" << arg_string[invalid_character_index-1] << "', but '"
                                                                        << arg_string[invalid_character_index-1] << "' is flag and therefore can't have an argument.\n";
                            }
                        }
                        print_throw_or_do_nothing(INVALID_FLAG, invalid_character_in_flag_group_message.str(), "Invalid flag(s) will be ignored\n\n");
                    }

                }
                else { // positional arg
                    if(arg_string == "help") {
                        std::cout << get_documentation_string_callback(subcommand_name, default_top_level_help_verbosity, default_subcommand_help_verbosity, default_help_recursion_level, default_hide_help_status);
                        return {{}, true};
                    }
                    else if(in_namespace) {
                        std::cerr << '\"' << current_subcommand_name_string << "\" is a namespace, so the only inputs it can accept are --help, -h, or help. The given input \"" << arg_string << "\" will therefore be ignored\n";
                        invalid_input_to_namespace = true;
                        continue;
                    }
                    else {
                        disambiguate_next_arg = false;
                        args.positional_args.emplace_back(arg_string);
                    }

                    const auto& docs = subcommand_name_to_docs()[commands.back().name];
                    const auto& command = commands.back();

                    unsigned expected_positionals_count = docs.positionals.size(),
                             actual_positionals_count   = args.positional_args.size();

                    if(!docs.variadic && (actual_positionals_count > expected_positionals_count)) {

                        std::stringstream ss;
                        ss << "Unexpected positional argument \"" << arg_string
                           << "\" given to " << (command.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(command.name) << '\n';/* <<
                           "\" (expected " << expected_positionals_count << ", got " << actual_positionals_count << ").\n";*/

                        ss << "It's also possible that \"" << arg_string
                           << "\" was supposed to be a subcommand, but was not recognized. "
                              "You can run\n" << to_string(command.name, " ") << " help\nto view the available subcommands for this command\n";

                        minor_error_type e;
                        if(optional_argument_option_with_no_value_provided_arg_index_to_option_string.contains(arg_i-1)) {
                            const std::string& erroneous_option_string = optional_argument_option_with_no_value_provided_arg_index_to_option_string.at(arg_i-1);
                            ss << "\nAnother possibility is that \"" << arg_string << "\" was supposed to be an argument for the previous commandline argument "
                               << '\"' << optional_argument_option_with_no_value_provided_arg_index_to_option_string.at(arg_i - 1) << '\"'
                               << ", which is an option that accepts an optional argument.\n"
                                  "However, an optional argument option can't be given an argument using the space separated syntax.\n"
                                  "The argument must use the '=' syntax (" << erroneous_option_string << '=' << arg_string << ')';

                            if(std::string(argv[arg_i-1]).substr(0,2) != "--") {
                                ss << " or, for (potentially grouped) short options, the connected syntax ("
                                   << erroneous_option_string << arg_string << ")";
                            }

                            ss << '\n';

                            e = EXCESS_POSITIONAL_WITH_SUSPICIOUS_OPTION;
                        }
                        else {
                            e = EXCESS_POSITIONAL;
                        }


                        print_throw_or_do_nothing(e, ss.str(), "This argument will be ignored\n\n");
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
                std::cout << '\"' << to_string(commands.back().name)
                          << "\" is a namespace, so using it without further subcommands doesn't do anything. Here is its help page: \n";
                //}
                std::cout << get_documentation_string_callback(commands.back().name, default_top_level_help_verbosity, default_subcommand_help_verbosity,
                                                                 default_help_recursion_level, default_hide_help_status);
                return {{}, true};
            }
        }

        ret.subcommands = std::move(commands);
        return ret;
    }
}
//end of "arg_parsing.cpp" include

//included from file "documentation.cpp"


namespace cppli {
    constinit documentation_verbosity default_top_level_help_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
    constinit documentation_verbosity default_subcommand_help_verbosity = NAME_AND_DESCRIPTION;
    constinit unsigned default_help_recursion_level          = -1;
    constinit bool     default_hide_help_status              = true;


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

    namespace detail {
        std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs() {
            static std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t> subcommand_name_to_docs_;

            return subcommand_name_to_docs_;
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

        std::string get_documentation_string_impl(const subcommand_name_t& name,
                                                  const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                  unsigned max_recursion_level,
                                                  unsigned current_recursion_level,
                                                  bool hide_help,
                                                  const std::optional<name_and_description_t>& main_command_override_name_and_description = std::nullopt) {

            const documentation_verbosity& verbosity = ((current_recursion_level == 0) ? top_level_verbosity : subcommand_verbosity);

            #define FOUR_SPACES "    "
            #define EIGHT_SPACES "        "

            using namespace detail;
            const auto& docs = subcommand_name_to_docs()[name];

            std::string indent = std::string(current_recursion_level*8, ' ');
            std::string ret = indent;

            if(main_command_override_name_and_description.has_value()) {
                ret += "Options or arguments surrounded by square brackets are optional, ones surrounded by angular brackets are required.\n"
                       "Arguments of the form [arg:type...] are variadic (as indicated by the \"...\") and can receive any number of arguments (including 0)\n\n";
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
                            ret += '\n';
                        }
                    }
                }
            }

            if(current_recursion_level+1 <= max_recursion_level) {
                if((docs.subcommands.size() > 1) || !hide_help || ((docs.subcommands.size() > 0) && (!docs.subcommands.contains("help")))) {
                    ret += indent;
                    ret += FOUR_SPACES "Subcommands:\n";

                    std::vector subcommand_name = name;
                    subcommand_name.resize(subcommand_name.size()+1);
                    for(const auto& e : docs.subcommands) {
                        subcommand_name.back() = e;
                        if((!hide_help) || (subcommand_name.back() != "help")) {
                            ret += get_documentation_string_impl(subcommand_name, top_level_verbosity, subcommand_verbosity, max_recursion_level, current_recursion_level + 1, hide_help);

                            if((subcommand_verbosity != NAME_ONLY) &&
                               (subcommand_verbosity != NAME_AND_ARGS) &&
                               (&e != &*(--docs.subcommands.end()))) {
                                    ret += indent;
                                    ret += '\n';
                            }
                        }
                    }
                }
            }

            #undef FOUR_SPACES
            #undef EIGHT_SPACES

            return ret;
        }
    }

    const subcommand_documentation_t& get_command_docs_from_name(const subcommand_name_t& name) {
        return detail::subcommand_name_to_docs().at(name);
    }

    std::string default_get_documentation_string_callback(const subcommand_name_t& name,
                                                          const documentation_verbosity& top_level_verbosity, const documentation_verbosity& subcommand_verbosity,
                                                          unsigned max_recursion_level,
                                                          bool hide_help) {
        if((name == subcommand_name_t{"MAIN"}) ||
           (name == subcommand_name_t{})) {
            return detail::get_documentation_string_impl({"MAIN"}, top_level_verbosity, subcommand_verbosity, max_recursion_level, 0, hide_help, detail::name_and_description_t{detail::program_name(), detail::program_description()});
        }
        else {
            return detail::get_documentation_string_impl(name, top_level_verbosity, subcommand_verbosity, max_recursion_level, 0, hide_help);
        }
    }

    constinit get_documentation_string_t get_documentation_string_callback = default_get_documentation_string_callback;

    /*std::string default_get_documentation_string_callback(documentation_verbosity verbosity, unsigned max_recursion_level, bool hide_help) {
        return detail::get_documentation_string_impl({"MAIN"}, verbosity, max_recursion_level, 0, detail::name_and_description_t{detail::program_name(), detail::program_description()});
    }*/
}
//end of "documentation.cpp" include

//included from file "subcommand.cpp"

#include <tuple>
#include <sstream>


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

    std::unordered_map<subcommand_name_t, subcommand_error_checking_func_t, subcommand_name_hash_t>& subcommand_name_to_error_checking_func() {
        static std::unordered_map<subcommand_name_t, subcommand_error_checking_func_t , subcommand_name_hash_t> subcommand_name_to_error_checking_func_;

        return subcommand_name_to_error_checking_func_;
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
        return
            subcommand_name_to_inputs_info().contains(subcommand) &&
            subcommand_name_to_inputs_info().at(subcommand).flags.contains(flag_name);
    }

    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name) {
        return
            subcommand_name_to_inputs_info().contains(subcommand) &&
            subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name);
    }

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name) {
        if(subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.contains(option_name)) {
            return subcommand_name_to_inputs_info().at(subcommand).option_argument_is_optional.at(option_name);
        }
        return false;
    }

    void error_if_flag_or_option_already_included(const subcommand_t& subcommand, const std::string& flag_or_option) {
        const auto& inputs_info = subcommand_name_to_inputs_info().at(subcommand.name);
        std::optional<std::reference_wrapper<const std::string>> short_name;
        if(inputs_info.flag_or_option_long_name_to_short_name.contains(flag_or_option)) {
            short_name = std::ref(inputs_info.flag_or_option_long_name_to_short_name.at(flag_or_option));
        }

        std::stringstream ss;

        if(inputs_info.flags.contains(flag_or_option)) {
            if(subcommand.inputs.flags.contains(flag_or_option) ||
               (short_name && subcommand.inputs.flags.contains(*short_name))) {

                ss << (subcommand.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " flag --" << flag_or_option << " included multiple times";

                if(short_name && subcommand.inputs.flags.contains(*short_name)) {
                    ss << "(previously included with short name '" << short_name->get() << '\'';
                }

                print_throw_or_do_nothing(FLAG_INCLUDED_MULTIPLE_TIMES, ss.str());
            }
        }
        else {
            if(subcommand.inputs.options_to_values.contains(flag_or_option) ||
                    (short_name && subcommand.inputs.options_to_values.contains(*short_name))) {

                ss << (subcommand.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " option --" << flag_or_option << " included multiple times";

                if(short_name && subcommand.inputs.options_to_values.contains(*short_name)) {
                    ss << "(previously included with short name '" << short_name->get() << '\'';
                }

                print_throw_or_do_nothing(OPTION_INCLUDED_MULTIPLE_TIMES, ss.str(), "\nThe value of the first instance of this option will be used, and all other instances will be ignored\n");
            }
        }
    }

    void error_if_short_flag_or_option_already_included(const subcommand_t& subcommand, const std::string& short_flag_or_option) {
        const auto& inputs_info = subcommand_name_to_inputs_info().at(subcommand.name);
        const std::string& long_name = inputs_info.flag_or_option_short_name_to_long_name.at(short_flag_or_option[0]);

        std::stringstream ss;

        if(inputs_info.flags.contains(short_flag_or_option)) {
            if(subcommand.inputs.flags.contains(short_flag_or_option) ||
               subcommand.inputs.flags.contains(long_name)) {

                ss << (subcommand.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " flag --" << short_flag_or_option << " included multiple times";

                if(subcommand.inputs.flags.contains(long_name)) {
                    ss << "(previously included with long name \"" << long_name << '\"';
                }
                print_throw_or_do_nothing(FLAG_INCLUDED_MULTIPLE_TIMES, ss.str());
            }

        }
        else {
            if(subcommand.inputs.options_to_values.contains(short_flag_or_option) ||
               subcommand.inputs.options_to_values.contains(long_name)) {

                ss << (subcommand.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << ' ' << to_string(subcommand.name)
                   << " option --" << short_flag_or_option << " included multiple times";

                if(subcommand.inputs.options_to_values.contains(long_name)) {
                    ss << "(previously included with long name \"" << long_name << '\"';
                }

                print_throw_or_do_nothing(OPTION_INCLUDED_MULTIPLE_TIMES, ss.str(), "\nThe value of the first instance of this option will be used, and all other instances will be ignored\n");
            }
        }
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

    std::string to_string(const subcommand_name_t& name, const char* delimiter) {
        std::string ret;

        if(name.size()) {
            if(name.front() == "MAIN") {
                ret += program_name();
            }
            else {
                ret += name.front();
            }
            if(name.size() > 1) {
                ret += delimiter;
            }
        }
        for(unsigned i = 1; i < name.size()-1; ++i) {
            ret += name[i];
            ret += delimiter;
        }
        if(name.size() > 1) {
            ret += name[name.size()-1];
        }

        return ret;
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

//included from file "user_error.cpp"


namespace cppli {
    user_error::user_error(const std::string& what, enum minor_error_type  e) : std::runtime_error(what), error_variant_(e) {}
    user_error::user_error(const std::string& what, enum major_error_type  e) : std::runtime_error(what), error_variant_(e) {}
    user_error::user_error(const std::string& what, const error_variant_t& e) : std::runtime_error(what), error_variant_(e) {}

    const user_error::error_variant_t& user_error::error_type() const {
        return error_variant_;
    }
}
//end of "user_error.cpp" include

//included from file "command_registration.cpp"


namespace cppli::detail {
    void default_help_callback(const command_context_t& cppli_current_command,

                               const variadic<std::string, conversion_t<std::string>, false, "subcommand name", "The name of the subcommand to print help for."
                                                                                                                "If no subcommand is provided, then help is printed for the parent command">&, std::vector<std::string> subcommand_name,

                               const flag<"name-only", "only print subcommand names">&,                                  bool name_only,
                               const flag<"name-and-description", "print subcommand name and description">&,             bool name_and_description,
                               const flag<"name-and-args", "print subcommand name and args">&,                           bool name_and_args,
                               const flag<"name-description-and-args", "print subcommand name, description, and args">&, bool name_description_and_args,
                               const flag<"verbose", "print subcommand name and description", 'v'>&,                     bool verbose,
                               const flag<"hide-help", "don't show help when printing subcommands">&,                    bool hide_help,
                               const flag<"show-help", "do show help when printing subcommands">&,                       bool show_help,

                               const flag<"subcommands-name-only", "only print subcommand names">&,                                  bool subcommands_name_only,
                               const flag<"subcommands-name-and-description", "print subcommand name and description">&,             bool subcommands_name_and_description,
                               const flag<"subcommands-name-and-args", "print subcommand name and args">&,                           bool subcommands_name_and_args,
                               const flag<"subcommands-name-description-and-args", "print subcommand name, description, and args">&, bool subcommands_name_description_and_args,
                               const flag<"subcommands-verbose", "print subcommand name and description">&,                          bool subcommands_verbose,

                               const option<unsigned, conversion_t<unsigned>, false, "recursion", "how many levels of nested subcommands to print. 0 prints none", "unsigned integer", true, false, 'r'>&, const std::optional<unsigned>& recursion) {

        extern subcommand_name_t last_subcommand_;

        documentation_verbosity top_level_verbosity;
        if(verbose) {
            top_level_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(name_description_and_args) {
            top_level_verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(name_and_args) {
            top_level_verbosity = NAME_AND_ARGS;
        }
        else if(name_and_description) {
            top_level_verbosity = NAME_AND_DESCRIPTION;
        }
        else if(name_only) {
            top_level_verbosity = NAME_ONLY;
        }
        else {
            top_level_verbosity = default_top_level_help_verbosity;
        }

        documentation_verbosity subcommand_verbosity;
        if(subcommands_verbose) {
            subcommand_verbosity = NAME_DESCRIPTION_AND_ARGS_WITH_ARG_DESCRIPTIONS;
        }
        else if(subcommands_name_description_and_args) {
            subcommand_verbosity = NAME_DESCRIPTION_AND_ARGS;
        }
        else if(subcommands_name_and_args) {
            subcommand_verbosity = NAME_AND_ARGS;
        }
        else if(subcommands_name_and_description) {
            subcommand_verbosity = NAME_AND_DESCRIPTION;
        }
        else if(subcommands_name_only) {
            subcommand_verbosity = NAME_ONLY;
        }
        else {
            subcommand_verbosity = default_subcommand_help_verbosity;
        }

        if(hide_help && show_help) {
            std::cerr << "\nhide help and show help are mutually exclusive. Help will be shown\n";
        }

        if(subcommand_name.size()) {
            subcommand_name.insert(subcommand_name.begin(), "MAIN");
            std::cout << get_documentation_string_callback(subcommand_name, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
        }
        else {
            std::cout << get_documentation_string_callback(last_subcommand_, top_level_verbosity, subcommand_verbosity, recursion.value_or(default_help_recursion_level), (default_hide_help_status || hide_help) && !show_help);
        }
    }
}

//end of "command_registration.cpp" include

//included from file "command_macros.cpp"


namespace cppli::detail {
    subcommand_name_t last_subcommand_;
}
//end of "command_macros.cpp" include

//included from file "run.cpp"


namespace cppli::detail {
    extern subcommand_name_t last_subcommand_;

    void run_impl_(int argc, const char* const* const argv) {
        {
            subcommand_name_t main_help = {"MAIN", "help"};
            if(!subcommand_name_to_func().contains(main_help)) {
                register_command<default_help_callback>(main_help, "print help for this command", true);
            }
        }

        auto parse_ret = detail::parse(argc, argv);

        if(parse_ret.help_command_index && !parse_ret.printed_help) {
            // help command is special. Skip all other execution if it is encountered
            const auto& help_command = parse_ret.subcommands[*parse_ret.help_command_index];
            (detail::subcommand_name_to_func()[help_command.name])(help_command, {true});
        }                                                                         // help is always leaf
        else if(!parse_ret.printed_help) {
            const auto& commands_vec = parse_ret.subcommands;

            #ifdef CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN
                for(const auto& command : commands_vec) { // throws if any errors would occur calling the given commands, without actually calling them
                    (detail::subcommand_name_to_error_checking_func()[command.name])(command);
                }
            #endif


            bool runnable_command_found = false;

            if(!detail::subcommand_name_to_docs()[{"MAIN"}].is_namespace) {
                (detail::subcommand_name_to_func()[{"MAIN"}])(commands_vec[0], {(commands_vec.size() < 2)});
                runnable_command_found = true;
                detail::last_subcommand_ = {};
            }

            for(unsigned i = 1; i < commands_vec.size(); ++i) {
                detail::last_subcommand_ = commands_vec[i-1].name;
                if((detail::subcommand_name_to_func().contains(commands_vec[i].name))) {
                    runnable_command_found = true;
                    (detail::subcommand_name_to_func()[commands_vec[i].name])(commands_vec[i], {(i == commands_vec.size()-1)});
                }
            }

            if(!runnable_command_found) {
                std::cerr << "The input did not form any runnable commands\n";
                // TODO: print help here?
            }
        }
    }
}
//end of "run.cpp" include
#endif



