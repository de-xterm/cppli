# Positional separators
Sometimes you want to provide positional arguments that "looks like" a flag or an option.  
An example is trying to use `mv` with a file named "-f"; 
on my machine, trying to do`mv -f file` prints out `mv: missing destination file operand after 'f'`, 
because `mv` think "-f" is referring to the flag `-f`.
However, if I do `mv -- -f file`, everything works as expected.  

The `--` is called a positional separator. It simply  says "treat everything after the `--` as a positional argument, even if it looks like a flag or option (starts with `-` or `--`)."  

## Trouble with subcommands
In the simplest case, this works. However, it isn't exactly clear how the positional separator should interact with subcommands.
Say you have a program `program` that takes a filename as a positional argument, and has a subcommand `subcommand`.
What if we wanted to provide program with a file named "subcommand"? 
We could simply extend the behavior of the positional separator to include disambiguation of arguments that match subcommand names, but then we wouldn't be able to use any subcommands!
Under that behavior, `program -- subcommand` would execute `program` with the filename `subcommand`, 
but `program -- subcommand subcommand` would execute program with 2 positionals args, both set to the string "subcommand".  
It's clear that we need a better solution

## Solution
To solve this issue, cppli uses 3 positional separators to cover all desired behaviors

### `--1`
Disambiguate only the next argument (hence the `1`).  
So for a program `program` that takes a single string positional and a flag with the short name `-f`, the input  
`program --1 -f -f`  
executes `program` with its positional arg set to the string "-f" and the flag `-f` set to true, because only the first `-f` is disambiguated.

### `--`
Disambiguates any number of arguments until a subcommand is encountered. The subcommand is not disambiguated.
For a program `program` which takes a two positionals, two flags, `-f` and `-b`, and a subcommand `subcommand`, the input  
`program -- -f -b subcommand`  
will execute `program` with its two positionals set to "-f" and "-b" and then call `subcommand`.
Note how subcommand is not disambiguated.

### `---`
The same as `--`, but **does** disambiguate subcommands.
So if we consider a program `program` that takes 3 positionals, the flags `-f` and `-f`, and has a subcommand `subcommand`, the input  
`program --- -f -b subcommand`  
will execute `program` with its three positionals set to "-f", "-b", and "subcommand", respectively.
`-f` and `-b` won't be set and `subcommand` won't be called.