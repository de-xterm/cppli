# Commands
CPPLI is a callback based library, 
which means the main way you interact with the library is by defining callbacks that cppli will call based on the input that the user gives to the commandline.
In cppli, C++ callbacks map to what I call _commands_. You're probably familiar with these


There are two types of commands in cppli: the main command, and subcommands.
The main command corresponds to the callback that is called whenever a program is run [(example)](../examples/0-echo.cpp).  
Subcommands correspond to additional callbacks that are run based on what input the user gives. 
If we use `git` as an example, `git` would be the main command, and `commit` would be a subcommand.  
Subcommands can be nested indefinitely, and multiple subcommands can be specified with the same name, 
so long as they appear in different namespaces

## Namespaces
Namespaces look like commands, but take no input (except for help) and have no corresponding callback.
They are purely organizational constructs. 