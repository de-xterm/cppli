# Commandline Rules

## Terms
First off, here's what I mean. To my understanding, a lot of these words have somewhat vague meanings, so I'm 
* Argument: when I say argument, I'm referring to the actual space separated strings passed to `main`; the things in `argv`
* Position Arguments: These are arguments passed to a (sub)command *not* preceded by `-` or `--`. I call them positional because their meaning is determined based on their relative order. In the command `mv f1 f2`, I would consider `f1` and `f2` to be positional args
* Flag: a boolean value specified with `-f` or `--force`. Flags don't take arguments
* Option:

## Flags and Options

### Flags
Work how you'd expect

### Options
#### Short
Short options can be specified with or without a space, so `program -c blue` and `program -cblue` are equivalent.  
An equals sign can also be used to connect the option name and value, like so: `program -c=blue`.  
This has the same meaning as the other two examples

#### Long
Long options work the same as short options, except that you can't omit the space between the option name and its value,  
so `program --color blue` and `program --color=blue` are both valid, but `program --colorblue` is not (it would be interpreted as a flag or option called "colorblue")

#### Optional Option Arguments

##### Optional Options and Optional Option Arguments

This table shows how optional options interact with optional option arguments

|                                     | The option itself is optional      | The option itself is required                                                                                                                                                    |
|-------------------------------------|------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **The option argument is optional** | nothing✔️ <br/> -c✔️<br/> -cblue✔️ | Making it illegal to not include an option but okay to include it with no value wouldn't make sense, so this combination isn't allowed<br/><br/>nothing❌<br/> -c✔️<br/> -cblue✔️ |
| **The option argument is required** | nothing✔️<br/> -c❌ <br/> -cblue✔️  | nothing❌<br/> -c❌<br/> -cblue✔️                                                                                                                                                  |





### Grouping
Flags can be grouped like this: 
`program -abc`  
The input above is equivalent to `program -a -b -c`

The last character in a group can be an option that can optionally be take an argument, like so:  
`program -abcoouput.txt`. All characters after the first character in the group that matches an option **with a required (non-optional) argument** are assumed to be the characters of that options value.
This means that there can be only one option 


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