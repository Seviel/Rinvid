cmake_minimum_required(VERSION 3.16.3)

project("rinvid")

set(CMAKE_CXX_STANDARD 17)

option(RINVID_BUILD_TESTS "Build Rinvid tests" ON)
set(RINVID_GTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest"
    CACHE PATH "Path to a local GoogleTest source checkout")

find_package(Freetype REQUIRED)

add_library(${PROJECT_NAME})
add_library(rinvid_extern OBJECT)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_include_directories(rinvid_extern PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

file(
  GLOB_RECURSE
  SOURCES
  "core/*.cpp"
  "gui/*.cpp"
  "platformers/*.cpp"
  "sound/*.cpp"
  "system/*.cpp"
  "util/*.cpp")

file(
  GLOB_RECURSE
  EXTERN_SOURCES
  "extern/stb_image.cpp"
  "extern/glad/src/*.c")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_sources(rinvid_extern PRIVATE ${EXTERN_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE $<TARGET_OBJECTS:rinvid_extern>)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -O3)
target_compile_options(rinvid_extern PRIVATE -O3)
add_definitions(-DSFML_STATIC)

execute_process(COMMAND uname OUTPUT_VARIABLE uname)
if(uname MATCHES "^MSYS" OR uname MATCHES "^MINGW")
  find_library(RINVID_OPENAL_LIBRARY NAMES openal openal32 REQUIRED)
  find_library(RINVID_FLAC_LIBRARY NAMES FLAC flac REQUIRED)

  target_link_libraries(
    rinvid
    PRIVATE sfml-audio-s
            sfml-window-s
            sfml-system-s
            opengl32
            gdi32
            winmm
            freetype
            ${RINVID_OPENAL_LIBRARY}
            vorbisenc
            vorbisfile
            vorbis
            ogg
            ${RINVID_FLAC_LIBRARY})
else()
  target_link_libraries(rinvid PRIVATE pthread GL freetype sfml-window
                                       sfml-system sfml-audio)
endif()

add_subdirectory(examples/full_demo)
add_subdirectory(examples/physix)
add_subdirectory(examples/primitive_shapes)
add_subdirectory(examples/screens)
add_subdirectory(examples/shaders)
add_subdirectory(examples/sprites)
add_subdirectory(examples/testing_grounds)

if(RINVID_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
