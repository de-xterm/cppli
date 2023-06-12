# Commandline Rules

## Terms
First off, here's what I mean. To my understanding, a lot of these words have somewhat vague meanings, so I'm 
* Argument: when I say argument, I'm referring to the actual space separated strings passed to `main`; the things in `argv`
* Position Arguments: These are arguments passed to a (sub)command *not* preceded by `-` or `--`. 
I call them positional because their meaning is determined based on their relative order. In the command `mv f1 f2`, I would call `f1` and `f2` to be positional args
* Flag: a boolean value specified with a hyphen followed by single character (`-f`) or two hyphens followed by two or more characters (`--force`). 
Flags don't take arguments (unlike options)
* Option: a value that 

## Flags and Options

### Options
#### Short
Short options can be specified with or without a space, so `program -c blue` and `program -cblue` are equivalent.  
An equals sign can also be used to connect the option name and value, like so: `program -c=blue`.  
This has the same meaning as the other two syntaxes

#### Long
Long options work the same as short options, except that you can't omit the space between the option name and its value,  
so `program --color blue` and `program --color=blue` are both valid, but `program --colorblue` is not (it would be interpreted as a flag or option called "colorblue")

#### Optional and Required Options
In real life software, options are usually (almost always?) optional, meaning that both of the following inputs are valid:  
`program --color=blue`  
`program // no color specified`  
However, cppli also provides the ability to make required options. 
These work by simply requiring that an option be specified;
if the `color` option of the hypothetical program above were required, then the second input would be invalid.

#### Optional Option Arguments
An option's argument can also be optional, like this:  
`program --color // omitted`  
`program --color=blue`  
`program --color blue // DOESN'T DO WHAT YOU EXPECT`  
Note that you have to use the `=` syntax (or, with short options, the connected [`-cblue`] syntax) with optional option arguments,
because if there were a space after the option, the parser would assume that the option argument had been omitted and that the following token argument is an unrelated positional argument.

So with the example inputs above, under assumption 1, `program --color` could simply be equivalent to `program --color=green`, or whatever the default color is  
Under assumption 2, the inclusion of `--color` could mean something more general like "do colorized output, but with what", but 

##### Optional Options and Optional Option Arguments

This table shows how optional options interact with optional option arguments.  
✔️ indicates that the input is valid,  
❌ indicates that it isn't

|                                     | The option itself is optional      | The option itself is required         |
|-------------------------------------|------------------------------------|---------------------------------------|
| **The option argument is optional** | nothing✔️ <br/> -c✔️<br/> -cblue✔️ | ~~*nothing❌<br/> -c✔️<br/> -cblue✔️~~ |
| **The option argument is required** | nothing✔️<br/> -c❌ <br/> -cblue✔️  | nothing❌<br/> -c❌<br/> -cblue✔️       |
*: required options with optional argument wouldn't make sense, so I don't allow them


## Positionals
### 

### Disambiguation
Occasionally, the user might want to provide a positional argument that looks like a flag/option or subcommand.  
In these cases `--` can be used to disambiguate the following argument. `--` just means "the next argument should be treated as a positional, even if it starts with `-` or `--` or matches a subcommand name".  
Example: say `program` takes a single positional argument that is supposed to be a file.
`program --bad-file-name.txt`  
In this case, the file name starts with `--`, so this input will be interpreted as trying to execute `program` with the flag/option `bad-file-name.txt`.  
In order to interpret `--bad-file-name.txt` as a positional argument, we just need to put `--` before it, like so:  
`program -- --bad-file-name.txt`  
Note that `--` can be used to disambiguate itself, so `program -- --` is valid, and indicates that `--` is interpreted as a positional arg

### Variadics

## Subcommands


## Argument Order
For each (sub)command, positional arguments are required to appear in one contiguous block,
and flags and options are required to appear in another contiguous block.  
So `program arg1 arg2 -f --color` and `program  -f --color arg1 arg2` are both valid, but `program arg1 -f arg2 --color` is not.  
I enforce this requirement in order to make parsing easier


Consider the following input: `program subcommand --color`  

In this example, `color` is an option with an optional argument. 
Given that information, the meaning of this command is unambiguous: 
execute `subcommand`, with `color` set to whatever its default value is.


Now consider this command: `program subcommand --color what-is-this-for`

This is a bit ambiguous! It isn't really clear whether `what-is-this-for` is meant to be an argument for `color`
or an argument for `subcommand`.  
The way I solve this is by assuming that the next 