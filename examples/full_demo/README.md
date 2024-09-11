# Demo

This is a demo that showcases most of Rinvid's capabilities.  

To run it, run the following command from inside Rinvid repo:

#### On Ubuntu

    bazel run //examples/full_demo:Rinvid

#### On Windows 10

    bazel.exe run //examples/full_demo:Rinvid --compiler=mingw-gcc --enable_runfiles  

Note: this will build it first and after successful build it will run the application.
