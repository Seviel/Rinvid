#!/bin/sh

# Count and print lines of code excluding extern package
# Run from root repo directory

cpp_loc=`find -iname "*.cpp" -not -path "./extern/*" | xargs cat | wc -l`
h_loc=`find -iname "*.h" -not -path "./extern/*" | xargs cat | wc -l`
vert_loc=`find -iname "*.vert" -not -path "./extern/*" | xargs cat | wc -l`
frag_loc=`find -iname "*.frag" -not -path "./extern/*" | xargs cat | wc -l`

echo ".cpp: " $cpp_loc
echo ".h: " $h_loc
echo ".vert: " $vert_loc
echo ".frag: " $frag_loc

echo "Total: "$(($cpp_loc + $h_loc + $vert_loc + $frag_loc))
