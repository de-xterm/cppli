
#include "documentation.h"

namespace cppli {
    namespace detail {
        std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t>& subcommand_name_to_docs() {
            static std::unordered_map<subcommand_name_t, subcommand_documentation_t, subcommand_name_hash_t> subcommand_name_to_docs_;

            return subcommand_name_to_docs_;
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
                                                                                                                  documentation(
                                                                                                                          documentation),
                                                                                                                  optional(optional) {}


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

        std::string get_documentation_string_impl(const detail::subcommand_name_t& name, documentation_verbosity verbosity, unsigned recursion, unsigned current_recursion_level) {

            #define FOUR_SPACES "    "
            #define EIGHT_SPACES "        "
            
            using namespace detail;
            const auto& docs = subcommand_name_to_docs()[name];

            std::string indent = std::string(current_recursion_level*8, ' ');
            std::string ret = indent;

            struct arg_name_and_docs_t {
                std::string name,
                            docs;

                inline arg_name_and_docs_t(const std::string& name, const std::string& docs) : name(name), docs(docs) {}
            };

            if(docs.is_namespace) {
                ret += "(Namespace) ";
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
                ret +=  "(Subcommand) ";
                std::vector<arg_name_and_docs_t> positional_doc_strings;
                std::vector<arg_name_and_docs_t> flag_doc_strings;
                std::vector<arg_name_and_docs_t> option_doc_strings;

                if (verbosity >= NAME_AND_ARGS) {
                    ret += docs.name;
                    ret += ' ';

                    for (const auto& e: docs.positionals) {
                        std::string temp = add_appropriate_brackets(e.name + ":" + e.type, e.optional);
                        ret += temp;
                        ret += ' ';

                        positional_doc_strings.emplace_back(std::move(temp), e.documentation);
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

                if ((verbosity == NAME_AND_DESCRIPTION) || (verbosity > NAME_AND_ARGS)) {
                    ret += '\n';
                    ret += indent;
                    ret += FOUR_SPACES "Description:\n";
                    ret += indent;
                    ret += EIGHT_SPACES;
                    ret += docs.description;
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

            if(current_recursion_level+1 <= recursion) {
                if(docs.subcommands.size()) {
                    // ret += '\n';
                    ret += indent;
                    ret += FOUR_SPACES "Subcommands:\n";
                    //ret += indent;


                    std::vector subcommand_name = name;
                    subcommand_name.resize(subcommand_name.size()+1);
                    for(const auto& e : docs.subcommands) {
                        subcommand_name.back() = e;

                        ret += get_documentation_string_impl(subcommand_name, verbosity, recursion, current_recursion_level+1);

                        ret += indent;
                        ret += '\n';
                    }
                }
            }

            return ret;
        }
    }

    std::string get_documentation_string(const detail::subcommand_name_t& name, documentation_verbosity verbosity, unsigned recursion) {
        return detail::get_documentation_string_impl(name, verbosity, recursion, 0);
    }
}