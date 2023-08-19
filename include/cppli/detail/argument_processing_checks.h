#pragma once

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
        //static_assert(!(argument_info_t<T>::has_short_name && argument_info_t<U>::has_short_name));

        if constexpr(argument_info_t<T>::has_short_name) {
            if constexpr(argument_info_t<U>::has_short_name) {
                if constexpr(sizeof...(Ts) > 0) {
                    return (T::short_name == U::short_name) || pack_contains_short_name_func<T, Ts...>();
                }
                else {
                    return (T::short_name == U::short_name);
                }
            }
            else {
                if constexpr(sizeof...(Ts) > 0) {
                    return pack_contains_short_name_func<T, Ts...>(); //
                }
                else {
                    return false;
                }
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
        //static_assert(!(argument_info_t<T>::has_long_name && argument_info_t<U>::has_long_name));

        if constexpr(argument_info_t<T>::has_long_name) {
            if constexpr(argument_info_t<U>::has_long_name) {
                if constexpr(sizeof...(Ts) > 0) {
                    return (T::name == U::name) || pack_contains_long_name_func<T, Ts...>();
                }
                else {
                    return (T::name == U::name);
                }
            }
            else {
                if constexpr(sizeof...(Ts) > 0) {
                    return pack_contains_long_name_func<T, Ts...>(); //
                }
                else {
                    return false;
                }
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