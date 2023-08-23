
/// Godbolt link:

#include <iostream>

#define CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN // see docs/10-errors.md for a full explanation of what this does
#define CPPLI_IMPLEMENTATION // define this in one source file before including cppli.h
#include "cppli.h"
//#include "cppli/run.h" // for run(), if we were using cppli as a static library
//#include "cppli/command_macros.h" // for CPPLI_MAIN_COMMAND, if we were using cppli as a static library


CPPLI_MAIN_COMMAND( // define a callback that will be called when the program is executed
        CPPLI_POSITIONAL(float, a, "first number to add"),
        CPPLI_POSITIONAL(float, b, "second number to add") // define positional parameters "a" and "b", both of type float
) { // body of the callback function

    std::cout << a+b << '\n'; // the actual functionality goes here
}

int main(int argc, char** argv) {
    try { // catch any exceptions thrown during parsing
        // program name
        cppli::run<"add", "adds two numbers together">(argc, argv);
    }                      // program description
    catch(cppli::user_error& e) {
        std::cerr << e.what() << '\n';
    }
}