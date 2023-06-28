#pragma once

#include "subcommand.h"
#include "documentation.h"

namespace cppli::detail {
    template<typename T>
    /*add_const_ref_if_string_t<T>::*/T process_argument(unsigned cumulative_positional_index, const subcommand_t& subcommand) {
        
        std::string main_command_or_subcommand;
        
        if(subcommand.name == subcommand_name_t{"MAIN"}) {
            main_command_or_subcommand = "main command";
        }
        else {
            main_command_or_subcommand = "subcommand";
        }

        std::string canonical_name = T::name.string();

        std::optional<std::string> short_name;

        if constexpr(T::has_short_name) {
            short_name = std::string{T::short_name};
        }

        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument
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
                    //arg_info_t::is
                }
                else {
                    if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                        if(!subcommand.inputs.options_to_values.at(canonical_name).has_value()) {
                            throw std::runtime_error(main_command_or_subcommand + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\" requires an argument, but one was not provided (expected an argument of type " + static_cast<std::string>(T::type_string.make_lowercase_and_convert_underscores()) + "."
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
                            throw std::runtime_error(main_command_or_subcommand + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + *short_name + "\" (full name \"." + canonical_name + "\") requires an argument, but one was not provided (expected an argument of type " + T::type_string.string() + "."
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
                if(!subcommand.inputs.options_to_values.contains(canonical_name) &&
                   (short_name && !subcommand.inputs.options_to_values.contains(*short_name))) {

                    throw std::runtime_error(main_command_or_subcommand + " \""  + subcommand_name_to_docs()[subcommand.name].name + "\" was not provided with required option \"" + canonical_name + '"');
                }
                else if(!subcommand.inputs.options_to_values.at(canonical_name).has_value() ||
                        (short_name && !subcommand.inputs.options_to_values.at(*short_name).has_value())) {

                    throw std::runtime_error(main_command_or_subcommand + " \""  + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\" requires an argument, but one was not provided "
                                                                                                                                         "(expected an argument of type " + T::type_string.make_lowercase_and_convert_underscores().string() + ')');
                }
                else {
                    if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                        try {
                            return {subcommand.inputs.options_to_values.at(canonical_name)};
                        }
                        catch(std::runtime_error& e) {
                            throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + canonical_name + "\". Details: " + e.what());
                        }
                    }
                    else { // has_short_name is guaranteed to be true at this point
                        try {
                            return {subcommand.inputs.options_to_values.at(*short_name)};
                        }
                        catch(std::runtime_error& e) {
                            throw std::runtime_error("Error initializing " + main_command_or_subcommand + " \"" + subcommand_name_to_docs()[subcommand.name].name + "\" option \"" + *short_name + "\" (full name \"." + canonical_name + "\"). Details: " + e.what());
                        }
                    }
                }
            }
        }
        else if constexpr(arg_info_t::is_flag) {
            return subcommand.inputs.flags.contains(canonical_name) ||
                   (short_name && subcommand.inputs.flags.contains(*short_name));
        }
        else /*if constexpr(arg_info_t::is_positional)*/ {
            if((cumulative_positional_index >= subcommand.inputs.positional_args.size())) {
                if(!T::optional) {
                    throw std::runtime_error(main_command_or_subcommand + " \""  + subcommand_name_to_docs()[subcommand.name].name + "\" required positional argument \"" + canonical_name + "\" was not provided (expected an argument of type " + T::type_string.string() + ')');
                }
                else {
                    return {};
                }
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
    }

    template<typename T_>
    struct type_wrapper { using T = T_; };

    template<std::size_t current_index, std::size_t desired_index, typename T, typename...Ts>
    static constexpr auto get_type_from_index_in_pack_func() {
        if constexpr(current_index == desired_index) {
            return type_wrapper<T>{};
        }
        else {
            static_assert(sizeof...(Ts) > 0, "index out of range");
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
    template<typename arg_t, typename...arg_ts>
    constexpr std::size_t count_positionals() {
        constexpr bool is_positional = argument_info_t<std::remove_cvref_t<arg_t>>::is_positional;
        if constexpr(sizeof...(arg_ts) > 0) {
            return count_positionals<arg_ts...>()+is_positional;
        }
        else {
            return is_positional;
        }
    }

    template<std::size_t max_index, typename...arg_ts>
    static constexpr std::size_t count_positionals_before_index_v = count_positionals_before_index_func<0, max_index, arg_ts...>();

    template<typename func_t, auto func, typename index_seq>
    struct call_func_wrapper_impl_t;

    template<typename return_t, typename...arg_ts, std::size_t...indices, auto func>
    struct call_func_wrapper_impl_t<return_t(*)(arg_ts...), func, std::integer_sequence<std::size_t, indices...>> {
    static void call_func(const subcommand_t& subcommand) {
        func(process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand)...);
    }
};

template<typename func_t, auto func>
struct call_func_wrapper_t;

template<typename return_t, typename...arg_ts, auto func>
struct call_func_wrapper_t<return_t(*)(arg_ts...), func> { // we need all this ugly partial specializations so that we can deduce arg_ts and indices
    static void call_func(const subcommand_t& subcommand) {
        constexpr auto positionals_count = count_positionals<arg_ts...>();
        if(positionals_count < subcommand.inputs.positional_args.size()) {
            std::cerr << "Too many positional arguments given to " << (subcommand.name == subcommand_name_t{"MAIN"} ? "main command" : "subcommand") << " \"" << subcommand_name_to_docs()[subcommand.name].name <<
                      "\" (expected " << std::to_string(positionals_count) << ", got " << std::to_string(subcommand.inputs.positional_args.size()) << "). Excess positional argument will be ignored\n";
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
    if constexpr(T::has_short_name && U::has_short_name) {
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
    if constexpr(T::has_long_name && U::has_long_name) {
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
    else { // positional
        documentation.positionals.emplace_back(type::type_string.string(),
                                               type::name.string(),
                                               type::documentation.string(),

                                               type::optional);
    }

    if constexpr(sizeof...(arg_ts) > 0) {
        generate_input_info_and_docs<return_t, arg_ts...>(info, documentation);
    }
}

inline void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, void(*func)() = nullptr) {
    // do nothing
}

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
        subcommand_name_to_docs()[name].is_namespace = false;

        // can't do a full assignment because that would wipe subcommands
    }
    else {
        subcommand_name_to_docs().insert_or_assign(name, std::move(docs));
    }

    return {};
}
}