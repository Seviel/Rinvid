# Rinvid

Rinvid is a small framework for 2D games and multimedia applications development. Although it's primary use is for me to gain some experience and learn a bit about OpenGl and perhaps if it turns out well, in the future it could be used as a starting point for all my game projects. It is written in C++ and uses OpenGl and SFML. It's developed on Ubuntu 20.04 so Linux is the only supported OS for now.  

## How to build it

### Ubuntu

These are instructions for Ubuntu 20.04, it would probably work on other Debian based distros:  
First, you will need to install Bazel and SFML. [Bazel](https://bazel.build/) is a build system used by Rinvid. [SFML](https://www.sfml-dev.org/) is a lib that provides a simple interface to the various components of your PC and is used by Rinvid.  

    sudo apt install npm  
    sudo npm install -g @bazel/bazelisk  
    sudo apt install libsfml-dev  

After that, clone the repo and inside any repo directory run:  
    
    bazel run //examples/testing_grounds:test

This will build and run a Rinvid test example. If it succeeds, you are all set.  

### Windows 10

What you need to build Rinvid on Windows 10:

   1. [MinGW](http://mingw-w64.org/doku.php)  
   2. [Bazel](https://bazel.build/)  
   3. [MSYS](https://www.msys2.org/)
   3. [SFML](https://www.sfml-dev.org/)  
   4. [CMake](https://cmake.org/download/) (needed only if you're gonna build SFML from source)  

#### Step 1
Install MinGW from this [link](http://mingw-w64.org/doku.php). Make sure the g++ compiler is selected when choosing which packages to install in MinGW installation manager. Note: if you don't want to build SFML from source, visit [this page](https://www.sfml-dev.org/download/sfml/2.5.1/) first and check which exact version of MinGW you need to install in order for SFML to work correctly.

#### Step 2
Install Bazel by following instructions from this [link](https://docs.bazel.build/versions/master/install-windows.html)

#### Step 3
Install MSYS2 by follwing instructions from this [link](https://www.msys2.org/) (only first 4 steps are needed)  

#### Step 4
Copy contents of folder where your MinGW installed, into <msys2_install_path>/mingw64  

#### Step 5
Add <msys2_install_path>/mingw64/bin to Path environment variable, and remove original MinGW installation location from Path env var.  

#### Step 6 (Only if building SFML from source)
Install CMake from this [link](https://cmake.org/download/)  

#### Step 7
Download SFML from this [page](https://www.sfml-dev.org/download/sfml/2.5.1/). If you're building SFML from source, follow instructions from this [link](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php)

#### Step 8
After having downloaded/built SFML, copy SFML folder containing headers to location where standard library includes are (on my machine it was: <msys2_install_path>/mingw64/include); Copy static SFML libs (.a files) to <msys2_install_path>/mingw64/lib; Add location of your SFML dynamic libs (.dll files) to Path env var.

#### Step 9
Finally, in terminal navigate to Rinvid root folder, and run the following command:

    bazel build ... --compiler=mingw-gcc

A note on running examples on Windows:
To be able to run examples that use resources (textures). You need to add an additional argument --enable_runfiles to bazel run. Example:

    bazel run //examples/testing_grounds:test --compiler=mingw-gcc --enable_runfiles  

In order for --enable_runfiles to work, you need to enable "developer mode" in Windows settings.

## External libraries used by Rinvid

[SFML](https://www.sfml-dev.org/)  
[stb_image](https://github.com/nothings/stb)  
[glm](https://github.com/g-truc/glm)  
