# Testing grounds

This is a special Rinvid example that is often updated. It is used for testing and showcasing latest features developed.

To run it, run the following command from inside Rinvid repo:

#### On Ubuntu

    bazel run //examples/testing_grounds:test

#### On Windows 10

    bazel.exe run //examples/testing_grounds:test --compiler=mingw-gcc --enable_runfiles  

Note: this will build it first and after successful build it will run the application.
