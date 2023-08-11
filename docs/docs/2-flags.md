# Flags
Flags represent a single boolean value

## CPPLI Syntax
```cpp                        
                                // short name is optional
CPPLI_FLAG(NAME,DOCUMENTATION,/*SHORT_NAME*/...)
```
this macro defines a `bool` parameter in a callback that is true when the corresponding flag is provided to the relevant command.

## Commandline Syntax
Flag can be specified with a long name (`--force`) or a short name (`-f`). 
If a flag is included (with its short or long name), the associated boolean parameter in the callback is set to true.
Otherwise, the boolean will be false. 

Providing the same flag multiple times is considered a [minor error](minor_errors.md)