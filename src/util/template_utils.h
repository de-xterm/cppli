#pragma once

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