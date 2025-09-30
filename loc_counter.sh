#!/bin/sh

# Count and print lines of code excluding extern package
# Run from root repo directory

if command -v tokei >/dev/null 2>&1; then
    tokei --exclude extern
elif command -v tokei.exe >/dev/null 2>&1; then
    tokei.exe --exclude extern
else
    cpp_loc=`find -iname "*.cpp" -not -path "./extern/*" | xargs cat | wc -l`
    cxx_loc=`find -iname "*.cxx" -not -path "./extern/*" | xargs cat | wc -l`
    h_loc=`find -iname "*.h" -not -path "./extern/*" | xargs cat | wc -l`
    hpp_loc=`find -iname "*.hpp" -not -path "./extern/*" | xargs cat | wc -l`
    vert_loc=`find -iname "*.vert" -not -path "./extern/*" | xargs cat | wc -l`
    frag_loc=`find -iname "*.frag" -not -path "./extern/*" | xargs cat | wc -l`
    sh_loc=`find -iname "*.sh" -not -path "./extern/*" | xargs cat | wc -l`
    py_loc=`find -iname "*.py" -not -path "./extern/*" | xargs cat | wc -l`

    echo ".cpp: " $cpp_loc
    echo ".cxx: " $cxx_loc
    echo ".h: " $h_loc
    echo ".hpp: " $hpp_loc
    echo ".vert: " $vert_loc
    echo ".frag: " $frag_loc
    echo ".sh: " $sh_loc
    echo ".py: " $py_loc

    echo "Total: "$(($cpp_loc + $cxx_loc + $h_loc + $hpp_loc + $vert_loc + $frag_loc + $sh_loc + $py_loc))
fi
