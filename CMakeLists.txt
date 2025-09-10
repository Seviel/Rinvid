cmake_minimum_required(VERSION 3.16.3)

project("rinvid")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER /mingw64/bin/gcc.exe)
set(CMAKE_CXX_COMPILER /mingw64/bin/g++.exe)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(
  GLOB_RECURSE
  SOURCES
  "core/*.cpp"
  "gui/*.cpp"
  "sound/*.cpp"
  "system/*.cpp"
  "util/*.cpp"
  "extern/stb_image.cpp"
  "extern/glad/src/*.c")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O3)

execute_process(COMMAND uname OUTPUT_VARIABLE uname)
if(uname MATCHES "^MSYS" OR uname MATCHES "^MINGW")
  target_link_libraries(
    rinvid
    PRIVATE opengl32
            winmm
            gdi32
            freetype
            sfml-window
            sfml-system
            sfml-audio)
else()
  target_link_libraries(rinvid PRIVATE GL freetype sfml-window sfml-system
                                       sfml-audio)
endif()

add_subdirectory(examples/testing_grounds)
