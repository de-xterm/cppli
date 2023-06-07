
#include "subcommand.h"

#include <unordered_set>

namespace cppli::detail {
    /// this is just boost::hash_combine, but I don't want to drag boost into this library just for hash_combine
    template<typename T>
    std::size_t hash_combine(std::size_t& seed, const T& val) {
        return (seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }
}

namespace std {
    template<typename T>
    struct hash<std::vector<T>> {
        std::size_t operator()(const auto& vec) const {
            std::size_t hash = 0;
            for(const auto& e : vec) {
                cppli::detail::hash_combine(hash, e);
            }

            return hash;
        }
    };
}

namespace cppli::detail {
    using subcommand_func_t = void(const subcommand_args_t&);

    struct subcommand_inputs_info_t {
        std::unordered_set<std::string> flags;
        std::unordered_map<std::string, bool> option_argument_is_optional;
    };

    struct subcommand_inputs_info_and_func_t {
        subcommand_inputs_info_t inputs_info;
        subcommand_func_t func;
    };

    std::unordered_map<subcommand_name_t, subcommand_inputs_info_and_func_t> subcommand_name_to_func_;

    bool is_valid_subcommand(subcommand_name_t& parent_command_names, const std::string& arg) {
        std::vector temp = parent_command_names;
        parent_command_names.push_back(arg);
        if(subcommand_name_to_func_.contains(temp)) {
            parent_command_names = std::move(temp);
            return true;
        }
        return false;
    }

    bool subcommand_takes_flag(const subcommand_name_t& subcommand, const std::string& flag_name) {
        return subcommand_name_to_func_.at(subcommand).inputs_info.flags.contains(flag_name);
    }

    bool subcommand_takes_option(const subcommand_name_t& subcommand, const std::string& option_name) {
        return subcommand_name_to_func_.at(subcommand).inputs_info.option_argument_is_optional.contains(option_name);
    }

    bool subcommand_option_argument_is_optional(const subcommand_name_t& subcommand, const std::string& option_name) {
        if(subcommand_name_to_func_.at(subcommand).inputs_info.option_argument_is_optional.contains(option_name)) {
            return subcommand_name_to_func_.at(subcommand).inputs_info.option_argument_is_optional.at(option_name);
        }
        return false;
    }
}