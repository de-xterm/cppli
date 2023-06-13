
#include "parameter_types.h"

/*
 * CLIC_MAIN_COMMAND
 *
 * CLIC_SUBCOMMAND(NAME, ARGS..., FLAGS..., OPTIONS...)
 * CLIC_SUBCOMMAND_INHERIT_INPUTS
 *
 * CLIC_SUBCOMMAND_WITH_SUBCOMMAND // TODO: use recursive macro to accomplish "overloading" so we don't need _WITH_SUBCOMMAND variants
 * CLIC_SUBCOMMAND_WITH_SUBCOMMAND_INHERIT_INPUTS
 *
 * CLIC_NAMESPACE
 *
 *
 CLIC_COMMAND(CLIC_ARG(int, arg1), CLIC_ARG(string, path), CLIC_FLAG_SHORT(force, f), CLIC_OPTIONAL(int, block_size, 30), CLIC_OPTIONAL_OPTION(float, width,
 */