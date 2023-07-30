# Tests

Contains all the tests. Uses gtest.

To run all tests, run the following command from inside Rinvid repo:

#### On Ubuntu

    bazel run //tests:rinvid_test

#### On Windows 10

    bazel.exe brun //tests:rinvid_test --compiler=mingw-gcc --enable_runfiles
