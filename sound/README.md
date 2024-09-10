# Sound

This package provides utilities for playing sound/music.  
So far it's just a simple wrapper around sfml audio module.

To build it, run the following command from inside Rinvid repo:

#### On Ubuntu

    bazel build //sound/...

#### On Windows 10

    bazel.exe build //sound/... --compiler=mingw-gcc  
