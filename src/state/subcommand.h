#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <optional>
#include <set>

#include "types.h"

namespace cppli::detail {
    struct subcommand_inputs_t {
        std::vector<std::string> positional_args;
        std::unordered_map<std::string, std::optional<std::string>> options_to_values;
        std::unordered_set<std::string> flags;
    };

    using subcommand_name_t = std::vector<std::string>;

    struct subcommand_t {
        subcommand_name_t name;
        subcommand_inputs_t inputs;
    };

    using subcommand_func_t = void(const subcommand_t&);

    struct flag_info_t {
        std::string name,
                    documentation;

        char short_name;

        bool operator<(const flag_info_t& rhs) const;
    };

    struct option_info_t {
        std::string type,
                    name,
                    argument_text,
                    documentation;

        bool is_optional,
             argument_is_optional;

        bool operator<(const option_info_t& rhs) const;
    };

    struct positional_info_t {
        std::string type,
                    name,
                    documentation;

        char short_name;

        bool optional;
    };

    struct subcommand_documentation_t {
        subcommand_name_t  name;

        std::set<flag_info_t>          flags; // using ordered set because we want to print commands alphabetically
        std::set<option_info_t>        options;
        std::vector<positional_info_t> positionals;

        std::set<std::string>          subcommands;

        bool operator<(const subcommand_documentation_t& rhs) const;
    };

    struct subcommand_inputs_info_t {
        std::unordered_set<std::string> flags;
        std::unordered_map<std::string, bool> option_argument_is_optional;
    };

    struct subcommand_inputs_info_and_func_t {
        subcommand_inputs_info_t inputs_info;
        subcommand_func_t func;
    };


    /// if arg appended to parent_command_names forms a valid subcommand,
    /// pushes back arg to parent_command_names and returns true.
    /// Otherwise, just returns false
    bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg);

    bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name);
    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name);

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name);


    template<typename T>
    /*add_const_ref_if_string_t<T>::*/T process_argument(unsigned cumulative_positional_index, const subcommand_t& subcommand) {

        constexpr auto canonical_name = T::name.make_lowercase_and_convert_underscores();

        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument
        if constexpr(arg_info_t::is_option) {
            if constexpr(T::optional) {
                if constexpr(T::argument_optional) {
                    if(subcommand.inputs.options_to_values.contains(canonical_name)) {
                        try {
                            return {subcommand.inputs.options_to_values.at(canonical_name)}; // no need for has_value check here; returning an empty optional is valid
                        }
                        catch(std::runtime_error& e) {
                            throw std::runtime_error("Error initializing (sub)command \"" + subcommand.name.back() + "\" option \"" + static_cast<std::string>(canonical_name) + "\". Details: " + e.what());
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
                            throw std::runtime_error("(sub)command \"" + subcommand.name.back() + "\" option \"" + static_cast<std::string>(canonical_name) + "\" requires an argument, but one was not provided (expected an argument of type " + static_cast<std::string>(T::type_string.make_lowercase_and_convert_underscores()) + "."
                                                     "Note that this option is optional, so it is valid to omit it entirely, but the option's argument is required, so if the option is provided, it must come with an argument");
                        }
                        else {
                            try {
                                return {subcommand.inputs.options_to_values.at(canonical_name)}; // no need for has_value check here; returning an empty optional is valid
                            }
                            catch(std::runtime_error& e) {
                                throw std::runtime_error("Error initializing (sub)command \"" + subcommand.name.back() + "\" option \"" + static_cast<std::string>(canonical_name) + "\". Details: " + e.what());
                            }
                        }
                    }
                }
            }
            else {
                if(!subcommand.inputs.options_to_values.contains(canonical_name)) {
                    throw std::runtime_error(std::string("(sub)command \"") + subcommand.name.back() + "\" was not provided with required option \"" + static_cast<std::string>(canonical_name) + '"');
                }
                else if(!subcommand.inputs.options_to_values.at(canonical_name).has_value()) {
                    throw std::runtime_error(std::string("(sub)command \"") + subcommand.name.back() + "\" option \"" + static_cast<std::string>(canonical_name) + "\" requires an argument, but one was not provided (expected an argument of type " + static_cast<std::string>(T::type_string.make_lowercase_and_convert_underscores()) + ')');
                }
                else {
                    try {      
                        return {subcommand.inputs.options_to_values.at(canonical_name)};
                    }
                    catch(std::runtime_error& e) {
                        throw std::runtime_error("Error initializing (sub)command \"" + subcommand.name.back() + "\" option \"" + static_cast<std::string>(canonical_name) + "\". Details: " + e.what());
                    }
                }
                // if !contains -> error
            }
        }
        else if constexpr(arg_info_t::is_flag) {
            return subcommand.inputs.flags.contains(canonical_name);
        }
        else if constexpr(arg_info_t::is_positional) {
            if((cumulative_positional_index > subcommand.inputs.positional_args.size()) && (!T::optional)) {
                throw std::runtime_error(std::string("(sub)command \"") + subcommand.name.back() + "\" required positional argument \"" + static_cast<std::string>(canonical_name) + "\" was not provided (expected an argument of type \"" + static_cast<std::string>(T::type_string.make_lowercase_and_convert_underscores()) + ')');
            }
            try {
                return {subcommand.inputs.positional_args[cumulative_positional_index]};
            }
            catch(std::runtime_error& e) {
                throw std::runtime_error("Error initializing (sub)command \"" + subcommand.name.back() + "\" positional argument \"" + static_cast<std::string>(canonical_name) + "\". Details: " + e.what());
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
            return count_positionals_before_index<current_index+1, max_index, arg_ts...>()+argument_info_t<std::remove_cvref_t<arg_t>>::is_positional;
        }
        else {
            return 0; // return 0 instead of is_positional because inclusivity is not what we want
        }
    }

    template<std::size_t max_index, typename...arg_ts>
    static constexpr std::size_t count_positionals_before_index_v = count_positionals_before_index_func<0, max_index, arg_ts...>();

    template<typename func_t, auto func, typename index_seq>
    struct call_func_wrapper_impl_t;

    template<typename return_t, typename...arg_ts, std::size_t...indices, auto func>
    struct call_func_wrapper_impl_t<return_t(arg_ts...), func, std::integer_sequence<std::size_t, indices...>> {
        static void call_func(const subcommand_t& subcommand) {
            func(process_argument<std::remove_cvref_t<get_type_from_index_in_pack<indices, arg_ts...>>>(count_positionals_before_index_v<indices, arg_ts...>, subcommand)...);
        }
    };

    template<typename func_t, auto func>
    struct call_func_wrapper_t;

    template<typename return_t, typename...arg_ts, auto func>
    struct call_func_wrapper_t<return_t(arg_ts...), func> { // we need all this ugly partial specializations so that we can deduce arg_ts and indices
        static void call_func(const subcommand_t& subcommand) {
            call_func_wrapper_impl_t<decltype(func), func, std::make_index_sequence<sizeof...(arg_ts)>>::call_func(subcommand);
        }
    };

    template<auto func>
    void call_func(const subcommand_t& command) {
        call_func_wrapper_t<decltype(func), func>::call_func(command);
    }

    template<typename return_t, typename arg_t, typename...arg_ts>                      // don't actually care about func, just need to deduce args
    void generate_input_info_and_docs(subcommand_inputs_info_t& info, subcommand_documentation_t& documentation, return_t(*func)(arg_t, arg_ts...) = nullptr) {
        using arg_info_t = argument_info_t<std::remove_cvref_t<arg_t>>;
        using type = std::remove_cvref_t<arg_t>;
        if constexpr(arg_info_t::is_flag) {
            documentation.options.emplace()
        }
        else if constexpr(arg_info_t::is_option) {

        }
        else { // positional

        }

        if constexpr(sizeof...(arg_ts) > 0) {
            generate_input_info_and_docs<return_t, arg_ts...>(info, documentation);
        }
    }

    template<auto func>
    void register_subcommand(const subcommand_name_t& name) {
        extern std::unordered_map<subcommand_name_t, subcommand_inputs_info_and_func_t> subcommand_name_to_func_;
        subcommand_inputs_info_t   info;
        subcommand_documentation_t docs;
        // TODO: ge

        subcommand_name_to_func_.emplace(name, call_func<func>);
    }
}