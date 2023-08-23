
/// Godbolt link:

#include <iostream>
#include <fstream>
#include <filesystem>

#define CPPLI_FULL_ERROR_CHECKING_BEFORE_RUN // see docs/10-errors.md for a full explanation of what this does
#define CPPLI_IMPLEMENTATION // define this in one source file before including cppli.h
#include "cppli.h"
//#include "cppli/run.h" // for run(), if we were using cppli as a static library
//#include "cppli/command_macros.h" // for CPPLI_MAIN_COMMAND, if we were using cppli as a static library


CPPLI_SUBCOMMAND(create,
                 "create a file",
                 CPPLI_POSITIONAL(std::string, filename, "name of the file"),
                 CPPLI_FLAG(force, "wipe the file if it already exists", f), // defines a boolean parameter for the callback
                 CPPLI_OPTION(std::string, contents, "STRING", "contents to write to the new file", c)) { // defines a std::optional<std::string> for the callback

    std::ofstream ofs;
    if(std::filesystem::exists({filename})) {
        if(force) {
            ofs.open(filename, std::ios_base::trunc);
            if(contents) {
                ofs << *contents;
            }
        }
        else {
            std::cout << "file already exists\n";
            if(contents) {
                std::cout << "the provided contents will be ignored\n";
            }
        }
    }
    else {
        ofs.open(filename);
        if(contents) {
            ofs << *contents;
        }
    }
}

CPPLI_SUBCOMMAND(delete,
                 "delete a file",
                 CPPLI_POSITIONAL(std::string, filename, "name of the file to delete")) {

    if(std::filesystem::exists({filename})) {
        std::remove(filename.c_str());
    }
    else {
        std::cerr << "file doesn't exist\n";
    }
}

CPPLI_SUBCOMMAND(update,
                 "update the contents of an existing file",
                 CPPLI_POSITIONAL(std::string, filename, "name of the file to update"),
                 CPPLI_POSITIONAL(std::string, contents, "new contents of the file")) {

    if(std::filesystem::exists({filename})) {
        std::ofstream ofs(filename, std::ios_base::trunc);
        ofs << contents;
    }
    else {
        std::cerr << "file doesn't exist\n";
    }
}

CPPLI_SUBCOMMAND(print,
                 "print the contents of a file",
                 CPPLI_POSITIONAL(std::string, filename, "file to print contents of")) {

    if(std::filesystem::exists({filename})) {
        std::ifstream ifs(filename);
        std::stringstream ss;
        ss << ifs.rdbuf();

        std::cout << ss.str() << '\n';
    }
    else {
        std::cerr << "file doesn't exist\n";
    }
}

int main(int argc, char** argv) {
    try { // catch any exceptions thrown during parsing
        // program name
        cppli::run<"filemanager", "silly little program for managing files">(argc, argv);
    }                      // program description
    catch(cppli::user_error& e) {
        std::cerr << e.what() << '\n';
    }
}