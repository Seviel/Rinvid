cmake_minimum_required(VERSION 3.16.3)

project("rinvid")

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(
  GLOB_RECURSE
  SOURCES
  "core/*.cpp"
  "gui/*.cpp"
  "platformers/*.cpp"
  "sound/*.cpp"
  "system/*.cpp"
  "util/*.cpp"
  "extern/stb_image.cpp"
  "extern/glad/src/*.c")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -O3)
add_definitions(-DSFML_STATIC)

execute_process(COMMAND uname OUTPUT_VARIABLE uname)
if(uname MATCHES "^MSYS" OR uname MATCHES "^MINGW")
  target_link_libraries(
    rinvid
    PRIVATE sfml-audio-s
            sfml-window-s
            sfml-system-s
            opengl32
            gdi32
            winmm
            freetype
            openal32
            vorbisenc
            vorbisfile
            vorbis
            ogg
            flac)
else()
  target_link_libraries(rinvid PRIVATE pthread GL freetype sfml-window
                                       sfml-system sfml-audio)
endif()

add_subdirectory(examples/full_demo)
add_subdirectory(examples/primitive_shapes)
add_subdirectory(examples/screens)
add_subdirectory(examples/shaders)
add_subdirectory(examples/sprites)
add_subdirectory(examples/testing_grounds)

add_subdirectory(tests)
