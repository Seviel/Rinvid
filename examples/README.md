# Examples

This is where examples of usage of Rinvid are. Each example is a small application that demonstrates how to use a part of Rinvid. There is also one special example that resides in the testing_grounds directory. This is used by Rinvid developers to try out latest features developed. Usually, upon developing a new feature or part of a feature, a small test example is added to testing_grounds application, and after the feature is considered complete, a dedicated example application should be made. 

To build all examples, run the following command from inside Rinvid repo:

#### On Ubuntu

    bazel build //examples/...

#### On Windows 10

    bazel.exe build //examples/... --compiler=mingw-gcc  

To build or run a specific example, look for instructions in the README.md file of that specific example.
