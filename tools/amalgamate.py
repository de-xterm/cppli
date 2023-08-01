#!/usr/bin/env python3

# this is a simple script for expanding a file's includes in order to produce a single header library
# it only considers includes made with quotes, not ones with angular brackets (so as not to expand standard includes)

# usage: amalgamate.py (takes no args, all relative paths are hardcoded)

# This script is hardcoded to fit the specific needs of this library, and it probably won't work for other projects without some troubleshooting





# TODO: don't expand commented out includes
# TODO: more error checks

import sys
import os
import pdb

def err(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    exit(1)


"""
if(len(sys.argv) < 4):
    err("incorrect number of args provided")
"""

input_header_path = "./cppli_single_header.h"
output_header_path = "../single_header/cppli.h"
include_dirs = ["../include/", "../src/"]


include_filenames_to_full_paths = {}

for dir in include_dirs
    for subdir, dirs, files in os.walk(dir):
        for file in files:
            include_filenames_to_full_paths[file] = os.path.join(subdir, file)
            include_filenames_to_full_paths[file] = os.path.join(subdir, os.path.basename(os.path.parent(file)) + "/" + file)

                print("Discovered file \""+file+'\"')

print("")

def expand_includes(filepath, already_expanded_files=set()):
    contents = ""

    file = open(filepath, "r")
    lines = file.readlines()

    for line in lines:
        if("#include" in line and '\"' in line):
            first_quote_index = line.index('\"')+1
            second_quote_index = line.index('\"', first_quote_index)

            include_filename = line[first_quote_index:second_quote_index]

            if(include_filename not in already_expanded_files):
                print("expanding file \""+include_filename+'\"')

                contents += "\n//included from file \""+include_filename+"\"\n"
                contents += expand_includes(include_filenames_to_full_paths[include_filename])
                contents += "\n//end of \""+include_filename+"\" include\n" 

                already_expanded_files.add(include_filename)
        elif("pragma once" not in line):
            contents += line

    return contents


output_text = "#pragma once\n\n"
output_text += expand_includes(input_header_path)
output_text += "\n\n"

print("successfully expanded includes\n")

out_file = open(output_header_path, "w")

out_file.write(output_text)

print("successfully wrote output file")
