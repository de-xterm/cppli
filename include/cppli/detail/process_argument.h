#pragma once

#include "parameter_types.h"

namespace cppli::detail {
    template<typename T, typename last, typename conversion_t_>
    T process_positional(unsigned cumulative_positional_index, const command_t& subcommand,
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
    T process_variadic(unsigned cumulative_positional_index, const command_t& subcommand,
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
    T process_required_option(const command_t& subcommand,
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
    T process_required_argument_option(const command_t& subcommand,
                                       const std::string& main_command_or_subcommand,
                                       const std::string& long_name, const std::optional<std::string>& short_name) {

        if (subcommand.inputs.options_to_values.contains(long_name)) {
            if (!subcommand.inputs.options_to_values.at(
                    long_name).has_value()) { // TODO: aren't we doing this check in arg_parsing.cpp too?
                throw user_error(main_command_or_subcommand + to_string(subcommand.name) +
                                 "\" option \"" + long_name + "\" "
                                                                   "requires an argument, but one was not provided (expected an argument of type "
                                 +
                                 conversion_t_::type_string.make_lowercase_and_convert_underscores().string() +
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
    T process_raw_type_optional_argument_option(const command_t& subcommand,
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
    T process_optional_argument_option(const command_t& subcommand,
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
    T process_raw_type(unsigned cumulative_positional_index, const command_t& subcommand,
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
    T process_wrapper(const command_t& subcommand,
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
    process_argument(unsigned cumulative_positional_index, const command_t& subcommand) {
        using arg_info_t = argument_info_t<T>; // no need for remove_cvref, we do that before calling process_argument

        std::string main_command_or_subcommand;

        if(subcommand.name == command_name_t{"MAIN"}) {
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
