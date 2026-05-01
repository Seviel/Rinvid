cmake_minimum_required(VERSION 3.16.3)

project("rinvid")

set(CMAKE_CXX_STANDARD 17)

option(RINVID_BUILD_TESTS "Build Rinvid tests" ON)
option(RINVID_BUILD_BENCHMARKS "Build Rinvid benchmarks" OFF)
set(RINVID_GTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest"
    CACHE PATH "Path to a local GoogleTest source checkout")
set(RINVID_BENCHMARK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/benchmark"
    CACHE PATH "Path to a local Google Benchmark source checkout")

find_package(Freetype REQUIRED)

add_definitions(-DSFML_STATIC)

function(rinvid_configure_target target)
  target_include_directories(
    ${target}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
           ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
           ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
           ${FREETYPE_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb)
  target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -O3)
endfunction()

file(GLOB CORE_SOURCES "core/*.cpp")
file(GLOB GUI_SOURCES "gui/*.cpp")
file(GLOB PLATFORMERS_SOURCES "platformers/*.cpp")
file(GLOB SOUND_SOURCES "sound/*.cpp")
file(GLOB SYSTEM_SOURCES "system/*.cpp")
file(GLOB UTIL_SOURCES "util/*.cpp")

file(GLOB EXTERN_SOURCES "extern/stb_image.cpp" "extern/glad/src/*.c")

add_library(rinvid_extern_obj OBJECT)
target_sources(rinvid_extern_obj PRIVATE ${EXTERN_SOURCES})
target_include_directories(rinvid_extern_obj
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(rinvid_extern_obj PRIVATE -O3)

add_library(rinvid_extern STATIC $<TARGET_OBJECTS:rinvid_extern_obj>)
target_include_directories(rinvid_extern
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(rinvid_extern PRIVATE -O3)

add_library(rinvid_util_obj OBJECT)
target_sources(rinvid_util_obj PRIVATE ${UTIL_SOURCES})
rinvid_configure_target(rinvid_util_obj)

add_library(rinvid_util STATIC $<TARGET_OBJECTS:rinvid_util_obj>)
rinvid_configure_target(rinvid_util)
target_link_libraries(rinvid_util PUBLIC rinvid_extern)

add_library(rinvid_core_obj OBJECT)
target_sources(rinvid_core_obj PRIVATE ${CORE_SOURCES})
rinvid_configure_target(rinvid_core_obj)

add_library(rinvid_core STATIC $<TARGET_OBJECTS:rinvid_core_obj>)
rinvid_configure_target(rinvid_core)
target_link_libraries(rinvid_core PUBLIC rinvid_util rinvid_extern freetype)

add_library(rinvid_platformers_obj OBJECT)
target_sources(rinvid_platformers_obj PRIVATE ${PLATFORMERS_SOURCES})
rinvid_configure_target(rinvid_platformers_obj)

add_library(rinvid_platformers STATIC $<TARGET_OBJECTS:rinvid_platformers_obj>)
rinvid_configure_target(rinvid_platformers)
target_link_libraries(rinvid_platformers PUBLIC rinvid_core rinvid_util)

add_library(rinvid_system_obj OBJECT)
target_sources(rinvid_system_obj PRIVATE ${SYSTEM_SOURCES})
rinvid_configure_target(rinvid_system_obj)

add_library(rinvid_system STATIC $<TARGET_OBJECTS:rinvid_system_obj>)
rinvid_configure_target(rinvid_system)
target_link_libraries(rinvid_system PUBLIC rinvid_core rinvid_util)

add_library(rinvid_gui_obj OBJECT)
target_sources(rinvid_gui_obj PRIVATE ${GUI_SOURCES})
rinvid_configure_target(rinvid_gui_obj)

add_library(rinvid_gui STATIC $<TARGET_OBJECTS:rinvid_gui_obj>)
rinvid_configure_target(rinvid_gui)
target_link_libraries(rinvid_gui PUBLIC rinvid_core rinvid_system rinvid_util)

add_library(rinvid_sound_obj OBJECT)
target_sources(rinvid_sound_obj PRIVATE ${SOUND_SOURCES})
rinvid_configure_target(rinvid_sound_obj)

add_library(rinvid_sound STATIC $<TARGET_OBJECTS:rinvid_sound_obj>)
rinvid_configure_target(rinvid_sound)

add_library(${PROJECT_NAME} STATIC $<TARGET_OBJECTS:rinvid_extern_obj>
                                   $<TARGET_OBJECTS:rinvid_util_obj>
                                   $<TARGET_OBJECTS:rinvid_core_obj>
                                   $<TARGET_OBJECTS:rinvid_platformers_obj>
                                   $<TARGET_OBJECTS:rinvid_system_obj>
                                   $<TARGET_OBJECTS:rinvid_gui_obj>
                                   $<TARGET_OBJECTS:rinvid_sound_obj>)
rinvid_configure_target(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC freetype)

execute_process(COMMAND uname OUTPUT_VARIABLE uname)
if(uname MATCHES "^MSYS" OR uname MATCHES "^MINGW")
  find_library(RINVID_OPENAL_LIBRARY NAMES openal openal32 REQUIRED)
  find_library(RINVID_FLAC_LIBRARY NAMES FLAC flac REQUIRED)

  target_link_libraries(rinvid_system PUBLIC sfml-window-s opengl32 gdi32)
  target_link_libraries(rinvid_sound PUBLIC sfml-audio-s sfml-system-s winmm
                                            ${RINVID_OPENAL_LIBRARY} vorbisenc vorbisfile vorbis
                                            ogg ${RINVID_FLAC_LIBRARY})
  target_link_libraries(${PROJECT_NAME} PUBLIC sfml-window-s opengl32 gdi32 sfml-audio-s
                                               sfml-system-s winmm
                                               ${RINVID_OPENAL_LIBRARY} vorbisenc vorbisfile
                                               vorbis ogg ${RINVID_FLAC_LIBRARY})
else()
  target_link_libraries(rinvid_system PUBLIC pthread GL sfml-window sfml-system)
  target_link_libraries(rinvid_sound PUBLIC sfml-audio sfml-system)
  target_link_libraries(${PROJECT_NAME} PUBLIC pthread GL sfml-window sfml-system sfml-audio)
endif()

add_subdirectory(examples/full_demo)
add_subdirectory(examples/perf_stress)
add_subdirectory(examples/physix)
add_subdirectory(examples/primitive_shapes)
add_subdirectory(examples/screens)
add_subdirectory(examples/shaders)
add_subdirectory(examples/sprites)
add_subdirectory(examples/testing_grounds)

if(RINVID_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

if(RINVID_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
