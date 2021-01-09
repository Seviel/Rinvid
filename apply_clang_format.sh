#!/bin/sh
clang-format-6.0 -i `find -iname *.cpp`
clang-format-6.0 -i `find . -type f \( -iname "*.h" ! -path "./extern/*" \)`
