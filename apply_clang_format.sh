#!/bin/sh

# Note: this script should be ran from repo root folder

clang-format-16 -i `find . -type f \( -iname "*.cpp" ! -path "./extern/*" \)`
clang-format-16 -i `find . -type f \( -iname "*.h" ! -path "./extern/*" \)`
