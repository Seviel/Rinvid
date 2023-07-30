[![Linux Ubuntu](https://github.com/Seviel/Rinvid/actions/workflows/ubuntu_ci.yml/badge.svg)](https://github.com/Seviel/Rinvid/actions/workflows/ubuntu_ci.yml) [![Windows](https://github.com/Seviel/Rinvid/actions/workflows/windows_ci.yml/badge.svg)](https://github.com/Seviel/Rinvid/actions/workflows/windows_ci.yml)

# Rinvid

Rinvid is a small framework for 2D games and multimedia applications development. Although it's primary use is for me to gain some experience and learn a bit about OpenGl and perhaps if it turns out well, in the future it could be used as a starting point for all my game projects. It is written in C++ and uses OpenGl and SFML. Ubuntu and Windows 10 are officially supported. 

## How to build it

### Ubuntu

These are instructions for Ubuntu 20.04, it would probably work on other Debian based distros:  
First, you will need to install Bazel and SFML. [Bazel](https://bazel.build/) is a build system used by Rinvid. [SFML](https://www.sfml-dev.org/) is a lib that provides a simple interface to the various components of your PC and is used by Rinvid.  
```shell
sudo apt install npm  
sudo npm install -g @bazel/bazelisk  
sudo apt install libsfml-dev  
sudo apt install libgtest-dev  
```
After that, clone the repo (run `git submodule update --init --recursive` after cloning to initiate submodules) and inside any repo directory run:  
```shell    
bazel run //examples/testing_grounds:test
```
This will build and run a Rinvid test example. If it succeeds, you are all set.  

### Windows 10

What you need to build Rinvid on Windows 10:

   1. [MinGW](https://www.mingw-w64.org/)  
   2. [Bazel](https://bazel.build/)  
   3. [MSYS](https://www.msys2.org/)
   4. [SFML](https://www.sfml-dev.org/)  
   5. [gtest](https://github.com/google/googletest)  

#### Development environment setup

1. Install [MSYS2](https://www.msys2.org/) - following all steps will also install MinGW via `pacman`
2. Additionally, install SFML and gtest via `pacman` by executing the following commands in the MSYS2 terminal:
```shell
pacman -S mingw-w64-x86_64-sfml
pacman -S mingw-w64-x86_64-gtest
```
3. Install [Bazel](https://bazel.build/install/windows) - consider also to put Bazel in your `PATH` environment variable so you can call the `bazel` from your terminal no matter the of the current path position.
4. Pull external dependencies by running the following command from the root of the project:
```shell
git submodule update --init --recursive
```

To verify your setup, run the following test example: 
```shell
bazel build //... --compiler=mingw-gcc
```
#### A note on running examples on Windows:
In order to run the examples that use resources (textures), you need to add an additional argument `--enable_runfiles` to the `bazel run` command, i.e:
```shell
bazel run //examples/testing_grounds:test --compiler=mingw-gcc --enable_runfiles  
```
And for `--enable_runfiles` to work, you need to enable [developer mode](https://docs.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development).

## External libraries used by Rinvid

[SFML](https://www.sfml-dev.org/)  
[stb_image](https://github.com/nothings/stb)  
[glm](https://github.com/g-truc/glm)  
[GLAD](https://glad.dav1d.de/)  
[gtest](https://github.com/google/googletest)
