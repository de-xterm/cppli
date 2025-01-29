#!/usr/bin/env python3

# this is a simple script for expanding a file's includes in order to produce a single header library
# it only considers includes made with quotes, not ones with angular brackets (so as not to expand standard includes)

# usage: amalgamate.py (takes no args, all paths are hardcoded)

# This script is hardcoded to fit the specific needs of this library, and it probably won't work for other projects without some troubleshooting





# TODO: don't expand commented out includes
# TODO: more error checks

import sys
import os
import pdb
from pathlib import Path



def err(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    exit(1)


input_header_path  = "./cppli_single_header_source.h"
output_header_path = "../single-header/cppli.h"
public_include_dirs       = ["../include/cppli", "../src/"]
private_include_dirs       = ["../include/cppli/detail", "../lib/iro", "../lib/ori", "../lib/utfcpp/source"]


include_filenames_to_full_paths = {}

#for dir in include_dirs:
  #  for files in os.walk(dir):
    #    for file in files:
           # if "detail" in subdir:
           #     filename = "detail/" + file
           # else:
           #     filename = file

           # include_filenames_to_full_paths[filename] = os.path.join(subdir, file)
            #include_filenames_to_full_paths[os.path.basename(subdir) + '/' + file] = os.path.join(subdir, file)

           # print("Mapping \"" + filename + "\" to file with path \"" + os.path.join(subdir, file)+'\"')

#print("")


def expand_includes(filepath, already_expanded_files=set()):
    contents = ""

    file = open(filepath, "r")
    lines = file.readlines()


    for line in lines:
        if("#include" in line and '\"' in line):
            #print("encountered include: ", line)
            first_quote_index = line.index('\"')+1
            second_quote_index = line.index('\"', first_quote_index)

            include_filename = line[first_quote_index:second_quote_index]

            #if ("src" in filepath) or ("detail" in filepath):
            #    include_filename = "detail/" + include_filename

            full_include_path = "" #check in the current directory first
            current_include_full_path_test = os.path.join(Path(filepath).parent.absolute(), include_filename)
            if os.path.isfile(current_include_full_path_test):
                full_include_path = current_include_full_path_test
            else:
                include_dirs=None
                if "src" in filepath:
                    include_dirs=private_include_dirs
                else:
                    include_dirs=public_include_dirs

                for include_dir in include_dirs:
                    current_include_full_path_test=os.path.join(include_dir,include_filename)
                    if os.path.isfile(current_include_full_path_test):
                        if full_include_path != "":
                            err("in file \"", filepath, "\" included filepath \"", include_filename, "\" is ambiguous. Matched \"", current_include_full_path_test, "\" and \"", full_include_path, "\"", sep='')
                        else:
                            full_include_path = current_include_full_path_test


            if full_include_path == "": #checked in every include directory and couldn't find the given file
                err("in file \"", filepath, "\" couldn't find included file \"", include_filename, "\"", sep='')


            if(Path(full_include_path).resolve() not in already_expanded_files):
                print("expanding file \""+include_filename+'\"')
                already_expanded_files.add(Path(full_include_path).resolve())

                contents += "\n//included from file \""+full_include_path+"\"\n"
                contents += expand_includes(full_include_path, already_expanded_files)
                contents += "\n//end of \""+include_filename+"\" include\n" 

        elif("pragma once" not in line):
            contents += line


    return contents


output_text = "#pragma once\n\n"
output_text += expand_includes(input_header_path)
output_text += "\n\n"

print("\nsuccessfully expanded includes")

out_file = open(output_header_path, "w")

out_file.write(output_text)

print("successfully wrote output file")
