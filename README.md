[![Linux Ubuntu](https://github.com/Seviel/Rinvid/actions/workflows/ubuntu_ci.yml/badge.svg)](https://github.com/Seviel/Rinvid/actions/workflows/ubuntu_ci.yml) [![Windows](https://github.com/Seviel/Rinvid/actions/workflows/windows_ci.yml/badge.svg)](https://github.com/Seviel/Rinvid/actions/workflows/windows_ci.yml)

![image](https://www.dreamscape.dev/projects/rinvid.png)

Rinvid is a small framework for 2D games and multimedia applications development. Although it's primary use is for me to gain some experience and learn a bit about OpenGl and perhaps if it turns out well, in the future it could be used as a starting point for all my game projects. It is written in C++ and uses OpenGl and SFML. Ubuntu and Windows 10 are officially supported. 

## How to build it

### Ubuntu

These are instructions for Ubuntu 20.04, it would probably work on other Debian based distros:  
First, you will need to install SFML, Ninja and Freetype.  
```shell
sudo apt install cmake
sudo apt install ninja-build
sudo apt install libsfml-dev  
sudo apt install libfreetype-dev  
```
Note that Freetpye headers must be in root include directory, and not in any new one. To make sure that's the case, you can run:  
```shell
cd /usr/include/freetype2  
sudo cp -r * ../  
```
After that, clone the repo (run `git submodule update --init --recursive` after cloning to initiate submodules) and then use CMake and Ninja to build the lib and all examples:  
```shell    
mkdir build
cd build
cmake -S .. -B . -G Ninja
ninja all
```

### Windows 10

What you need to build Rinvid on Windows 10:

   1. [MinGW](https://www.mingw-w64.org/)  
   2. [MSYS](https://www.msys2.org/)
   3. [SFML](https://www.sfml-dev.org/)  
   4. [gtest](https://github.com/google/googletest) source checkout
   5. [Freetype](http://freetype.org/)  
   6. [Ninja](https://ninja-build.org/)

#### Development environment setup

1. Install [MSYS2](https://www.msys2.org/) - following all steps will also install MinGW via `pacman`
2. Additionally, install CMake, Ninja, SFML and Freetype via `pacman` by executing the following commands in the MSYS2 terminal:
```shell
pacman -Syu
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-ninja
pacman -S mingw-w64-x86_64-sfml
pacman -S mingw-w64-x86_64-freetype
```
3. Pull external dependencies by running the following command from the root of the project:
```shell
git submodule update --init --recursive
```
4. Clone GoogleTest source into `extern/googletest` from the root of the Rinvid project:
```shell
git clone https://github.com/google/googletest.git extern/googletest
```
You can also point CMake to a different checkout with `-DRINVID_GTEST_SOURCE_DIR=/absolute/path/to/googletest`.

From the MinGW64 MSYS2 terminal, you can run the Ninja build procedure: 
```shell    
mkdir build
cd build
/mingw64/bin/cmake -S .. -B . -G Ninja
ninja all
```

To build without tests, configure with:
```shell
/mingw64/bin/cmake -S .. -B . -G Ninja -DRINVID_BUILD_TESTS=OFF
```

## External libraries used by Rinvid

[SFML](https://www.sfml-dev.org/)  
[stb_image](https://github.com/nothings/stb)  
[glm](https://github.com/g-truc/glm)  
[GLAD](https://glad.dav1d.de/)  
[gtest](https://github.com/google/googletest)  
[Freetype](http://freetype.org/)
