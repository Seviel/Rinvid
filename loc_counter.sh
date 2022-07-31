#!/bin/sh

# Count and print lines of code excluding extern package

cpp_loc=`find -iname "*.cpp" -not -path "./extern/*" | xargs cat | wc -l`
h_loc=`find -iname "*.h" -not -path "./extern/*" | xargs cat | wc -l`

echo ".cpp: " $cpp_loc
echo ".h: " $h_loc
echo "Total: "$(($cpp_loc + $h_loc))
