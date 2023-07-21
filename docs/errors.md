# Errors
CPPLI uses exceptions for errors. All exceptions are of the type `cppli::user_error` (user as in user of the commandline, not user of the library)
which is derived from `std::runtime_error`. `user_error` has a member function `error_type()` that can be used to query what kind error caused an exception

## String conversion errors and full vs progressive error checking
A string conversion error occurs when a user provided string cannot be converted to the appropriate type for a callback parameter.  
These errors are always fatal, but these errors are detected when calling a callback, not when initially parsing the program input.  
Because command callbacks are called in the order they are specified, this means that it's possible that an earlier callback could complete successfully, and a later one could fail due to a conversion error.  

**This behavior might be surprising, and could cause serious errors if one command allocates resources that need to be cleaned up by a later command.**  
  
Note that almost all actual software that uses subcommands only uses them as namespaces
(in `gh repo init`, `repo` isn't doing anything on its own, it's just the namespace that `init` lives in),
so the order of execution of subcommands isn't really an issue in practice (because there's almost always only ones "real" command).  

It's also probably not a great idea for maintainability to have important initialization and deinitialization separated by commands
(this would probably also imply inappropriate usage of global variables), 
but if you really *really* want to design you app this way, 
you can define `CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN` before including `command_macros.h`.
This will check all commands for conversion errors before calling any callbacks.  

Note that this might have side effects.
The full check is implemented by simply attempting all argument string conversions for all callbacks, and discarding the resulting object(s).
This means that if you have a parameter type that is expensive to construct/destruct, or whose constructor/destructor has important effects on global state,
then `CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN` could have effects on the performance or even _behavior_ of your program, so do use it with care

## Minor errors
Certain categories of errors are nonfatal. 
The behavior when encountering a nonfatal error can be configured by assigning to the result of `minor_error_behavior()` (note that it returns a reference), defined in [src/configuration/configuration.h](../src/configuration/configuration.h).
This can be set to one of the following enumerators:
  
`THROW`  
throw a user error explaining what happened
  
`MESSAGE`  
print a message to stderr explaining what happened  
  
`SILENT`  
do nothing (not recommended in most cases)  
  
The default behavior for all minor errors is `MESSAGE`