# CPPLI
cppli is a C++20 library for building commandline applications

## How to use
cppli is a callback based library.  
Perhaps the most notable feature of the callback system is that it uses macros to reuse the C++ identifier of the callback parameter as a string
```cpp

```

## Build
The library is available as a single header, or as a cmake static library  
  
### Single header build:
```shell
git clone --recurse https://github.com/de-xterm/cppli # you could also just download cppli/single_header/cppli.h from github
```
Add the file `cppli/single_header/cppli.h`, in a single source file, define ```CPPLI_IMPLEMENTATION``` *before* including `cppli.h`:
```cpp
#include<iostream>

#define CPPLI_IMPLEMENTATION
#include "cppli.h"
                              // this is both
                              // the flag name
                              // and it's c++
                              // identifier                // optional short name
CPPLI_MAIN_COMMAND(CPPLI_FLAG(do_thing), "force something to happen", d) { // macro magic generates the header of the function 
    if(force) {                       // documentation                  // and registers it, but the 
        std::cout << "doing thing\n";
    }
}

int main(int argc, char** argv) {
               // program name (for documentation)
    cppli::run<"program", "does stuff">(argc, argv);
}                         // program description
```
If the above program were executed like this:
```shell
program --do-thing # underscores in identifiers are converted to hyphens
```
or like this:
```shell
program -d
```
the output would be
```
doing thing
```
If the program was give no input
```shell
program
```
then nothing would be printed


### Static library build

## Why
* Very easy to use
* Very little typing
  * Uses macros to stringify 
* Lots of compile time checks for sane behavior
* Supports all common commandline features ([list of supported features](#features))
* Uses raw types as much as possible
  * Minimal use of wrapper classes
  * When wrappers are necessary, stl types are used wherever possible, so the library will be compatible with many existing algorithms
* Automatic documentation generation 
* Available as a single header for easy integration with your project

## Why not use existing libraries
* Too much code!
* No compile time checks
* The result of parsing is some weird wrapper class, not the raw type you asked for

## Commandline Features
* Flags
  * long names (`--flag`) and short names (`-f`)
  * short flags are groupable; `-fb` is equivalent to `--f --b`
* Options
  * All common syntaxes are supported
    * `--option value`
    * `--option=value`
    * `-o value`
    * `-o=value`
    * `-ovalue`
  * Options can appear at the end of a flag group
    * `-fbovalue` is equivalent to `-fb -ovalue`
  * Options with optional arguments
  * Required options
* Positional arguments
  * These can be optional
* Variadic positionals
  * Like a parameter pack, but for commandline args
* Positional separator `--`
  * This works a bit differently from usual, but for good reasons
* Subcommands
  * Nestable indefinitely

## Restrictions
* Long flags, long options, and subcommand names can only contain lowercase letters, numerals, and hyphens.
Short options and flags can be a lowercase or uppercase letter.
  * why?
    * because making the user touch the shift key is disrespectful
* Flags and options use GNU syntax
  * windows style `/option:value` is not allowed

## Why not 
* If you don't like macros
* If you want to use nonstandard
* If you want to change 