[![Linux Ubuntu](https://github.com/Seviel/Rinvid/actions/workflows/ubuntu_ci.yml/badge.svg)](https://github.com/Seviel/Rinvid/actions/workflows/ubuntu_ci.yml) [![Windows](https://github.com/Seviel/Rinvid/actions/workflows/windows_ci.yml/badge.svg)](https://github.com/Seviel/Rinvid/actions/workflows/windows_ci.yml)

![image](https://www.dreamscape.dev/projects/rinvid.png)

Rinvid is a small framework for 2D games and multimedia applications development. Although it's primary use is for me to gain some experience and learn a bit about OpenGl and perhaps if it turns out well, in the future it could be used as a starting point for all my game projects. It is written in C++ and uses OpenGl and SFML. Ubuntu and Windows 10 are officially supported. 

## How to build it

### Ubuntu

These instructions assume a distro with CMake 3.28 or newer and a compiler with C++17 filesystem
support. Older Ubuntu releases may need a newer CMake/compiler from Kitware or another package
source. First, install Ninja, Freetype, OpenGL, X11, and udev development packages. SFML is
provided by the `extern/sfml` submodule and built from source.
```shell
sudo apt install cmake
sudo apt install ninja-build
sudo apt install libfreetype-dev  
sudo apt install libgl1-mesa-dev
sudo apt install libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev
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

   1. [MinGW-w64](https://www.mingw-w64.org/)
   2. [MSYS2](https://www.msys2.org/)
   3. [gtest](https://github.com/google/googletest) source checkout
   4. [Freetype](http://freetype.org/)
   5. [Ninja](https://ninja-build.org/)

#### Development environment setup

1. Install [MSYS2](https://www.msys2.org/), then open the **MINGW64** MSYS2 terminal.
2. Additionally, install an up-to-date MinGW64 toolchain, CMake, Ninja and Freetype via
`pacman` by executing the following commands in the MINGW64 terminal. SFML 3.1 requires a compiler
with C++17 filesystem support; GCC 9 or newer is required.
```shell
pacman -Syu
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-ninja
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

From the MINGW64 MSYS2 terminal, you can run the Ninja build procedure:
```shell    
mkdir build
cd build
cmake -S .. -B . -G Ninja
ninja all
```

To build without tests, configure with:
```shell
cmake -S .. -B . -G Ninja -DRINVID_BUILD_TESTS=OFF
```

To enable benchmark builds during configure:
```shell
cmake -S .. -B . -G Ninja -DRINVID_BUILD_BENCHMARKS=ON
```

To enable Rinvid diagnostic logging and OpenGL error checks during configure:
```shell
cmake -S .. -B . -G Ninja -DRINVID_DEBUG_MODE=ON
```

To also append diagnostic messages to `rinvid_error.log`:
```shell
cmake -S .. -B . -G Ninja -DRINVID_DEBUG_MODE=ON -DRINVID_DEBUG_MODE_OUTPUT_TO_FILE=ON
```

SFML is built from the `extern/sfml` submodule. Rinvid builds SFML as shared libraries from source.
On Windows, CMake copies the SFML DLLs and the active MinGW runtime DLLs next to the built examples,
tests, and benchmarks.

The benchmark executable is `rinvid_benchmark`. The current sprite-scene benchmark generates random
RGBA textures in memory, with random dimensions between `50x50` and `500x500`, and uses Google
Benchmark command-line flags such as:
```shell
./benchmarks/rinvid_benchmark.exe --benchmark_dry_run
./benchmarks/rinvid_benchmark.exe --benchmark_filter=DrawRandomSpriteScene
./benchmarks/rinvid_benchmark.exe --benchmark_filter=FullStressFrame
./benchmarks/rinvid_benchmark.exe --benchmark_filter=FullStressFrameVary
```

There is also a runnable stress example app:
```shell
./examples/perf_stress/perf_stress.exe
```
It renders moving random textures, shapes, lights, and text, and lets you switch between `10`,
`25`, `50`, and `100` objects of each category with the `1`-`4` keys.
The benchmark suite also includes `FullStressFrameVarySprites`, `FullStressFrameVaryShapes`,
`FullStressFrameVaryText`, and `FullStressFrameVaryLights`, which scale one category while keeping
the other three pinned at `100`.

## External libraries used by Rinvid

[SFML](https://www.sfml-dev.org/)  
[stb_image](https://github.com/nothings/stb)  
[glm](https://github.com/g-truc/glm)  
[GLAD](https://glad.dav1d.de/)  
[gtest](https://github.com/google/googletest)  
[Google Benchmark](https://github.com/google/benchmark)  
[Freetype](http://freetype.org/)
