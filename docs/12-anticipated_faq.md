# Anticipated FAQ

**Why can't flags and options contain uppercase letters, underscores, or other symbols?**  
I don't think it's nice to make the user touch the shift key
  
**Why do all strings have to be compile-time constants?**  
Because an application that doesn't depend on external files (e.g., for documentation) will be more robust than one that does.  
Also it made a lot of stuff easier to implement :P  
Note that this implementation is still compatible with internationalization and localization. If want to support translated documentation, you can simply rewrite the [print documentation string callback](../include/cppli/detail/documentation.h) to fetch translated documentation at runtime using the compile-time constant documentation string as a key (using `gettext` or a similar tool)

**Why are C++ identifiers reused as the name for things (commands, flags, options, etc.) on the commandline?**  
Because it reduces the amount of typing a user of the library has to do, 
and I can't think of any reason why it would be beneficial to allow a C++ variable name to be different from the name of the thing it represents 
(why would anyone need to refer to the flag `force-reset` by anything other than `force_reset`?)

**Why doesn't `CPPLI_MAIN_COMMAND` take a name or description string (unlike `CPPLI_SUBCOMMAND`)?**  
Because `CPPLI_MAIN_COMMAND` is optional, 
if the description string for the program were provided in `CPPLI_MAIN_COMMAND`,
then programs where the main command is a namespace wouldn't have a name or description.
Forcing the program name and description to be provided in `cppli::run` solves this issue.

**Why use a single exception class with an `error_type()` member instead of using separate exception classes in a hierarchy?**  
Originally, I only included `error_type()` for testing purposes. After I added it, I figured there was no reason not to add it to the public API.  
During testing, I didn't actually need to catch exceptions of some types and let other types propagate; 
I just needed to make sure that an exception was thrown and that it had the correct error type

**Why does the library require C++20?**  
The only C++20 feature that I really _needed_ was `__VA_OPT__`, but I also use `constinit` 

**Does cppli support unicode**  
yes!  
all `char*`s and `std::string`s in the codebase are UTF-8. On windows, `CPPLI_MAIN_FUNCTION` calls `wmain` and the UTF-16 strings in `argv` are automatically converted to UTF-8. If you're supplying your own `main`, use `wmain` on windows and convert
Note it's still your responsibility to ensure that you use stdin and stdout in a unicode-friendly way