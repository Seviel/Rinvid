# Core

This is the main Rinvid package. It contains implementation of core features of Rinvid (graphics rendering).

To build it, run the following command from inside Rinvid repo:

#### On Ubunut

    bazel build //core/...

#### On Windows 10

    bazel build //core/... --compiler=mingw-gcc  

Note: core lib will automatically be built when building or running any of the examples in examples directory.
