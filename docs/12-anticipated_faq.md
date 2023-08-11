# Anticipated FAQ

## Why questions
**Why can't flags and options contain uppercase letters, underscores, or other symbols?**  
I don't think it's nice to make the user touch the shift key
  
**Why do all strings have to be compile time contants?**  
Because an application that doesn't depend on external files (e.g., for documentation) will be more robust than one that does.  
Also it made a lot of stuff easier to implement :P

**Why are C++ identifiers reused as the name for things (commands, flags, options, etc.) on the commandline?**  
Because it reduces the amount of typing a user of the library has to do, 
and I can't think of any reason why it would be beneficial to allow a C++ variable name to be different from the name of the thing it represents 
(why would anyone need to refer to the flag `force-reset` by anything other than `force_reset`?)

**Why do you use global variables?**  
Why not ;)
  
**Why are some global variables defined as functions that return a reference?**  
To provide variable-like syntax while avoiding the [Static Initialization Order Fiasco](https://en.cppreference.com/w/cpp/language/siof).  
I could have also used separate getters and setters, but they increase the amount of code without adding any value (in my opinion)
  
**Why use a single exception class with an `error_type()` member instead of using separate exception classes in a hierarchy?**  
Originally, I only included `error_type()` for testing purposes. After I added it, I figured there was no reason not to add it to the public API.  
During testing, I didn't actually need to catch exceptions of some types and let other types propagate; 
I just needed to make sure that an exception was thrown and that it had the correct error type

**Why does the library require C++20?**  
The only C++20 feature that I really _needed_ was `__VA_OPT__`, but I also use `constinit` 