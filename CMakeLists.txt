cmake_minimum_required(VERSION 3.28)

project("rinvid")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
  message(
    FATAL_ERROR
      "Rinvid requires GCC 9 or newer when building SFML 3.1 from source. "
      "Update the active compiler toolchain before configuring.")
endif()

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

option(RINVID_BUILD_TESTS "Build Rinvid tests" ON)
option(RINVID_BUILD_BENCHMARKS "Build Rinvid benchmarks" OFF)
option(RINVID_DEBUG_MODE "Enable Rinvid diagnostic logging and OpenGL error checks" OFF)
option(RINVID_DEBUG_MODE_OUTPUT_TO_FILE
       "Write Rinvid diagnostic logging output to rinvid_error.log" OFF)
set(RINVID_GTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest"
    CACHE PATH "Path to a local GoogleTest source checkout")
set(RINVID_BENCHMARK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/benchmark"
    CACHE PATH "Path to a local Google Benchmark source checkout")

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

set(SFML_BUILD_WINDOW ON)
set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_DOC OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_TEST_SUITE OFF)
set(SFML_USE_SYSTEM_DEPS OFF)

if(DEFINED BUILD_SHARED_LIBS)
  set(RINVID_PREVIOUS_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")
  set(RINVID_PREVIOUS_BUILD_SHARED_LIBS_WAS_DEFINED ON)
else()
  set(RINVID_PREVIOUS_BUILD_SHARED_LIBS_WAS_DEFINED OFF)
endif()

set(BUILD_SHARED_LIBS ON)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/sfml"
                 "${CMAKE_CURRENT_BINARY_DIR}/sfml"
                 EXCLUDE_FROM_ALL)

if(MINGW)
  # Keep SFML 3.1 building with the MSYS2 MINGW64/MSVCRT runtime.
  target_sources(sfml-window PRIVATE
                 "${CMAKE_CURRENT_SOURCE_DIR}/compat/mingw/thread_atexit.cpp")
  target_sources(sfml-audio PRIVATE
                 "${CMAKE_CURRENT_SOURCE_DIR}/compat/mingw/wcsrtombs.cpp")
  target_compile_options(sfml-audio PRIVATE -include
                                            "${CMAKE_CURRENT_SOURCE_DIR}/compat/mingw/wcsrtombs.h")
  target_compile_definitions(sfml-audio PRIVATE MA_HAS_WFOPEN
                                                wcsrtombs=rinvid_mingw_wcsrtombs)

  get_filename_component(RINVID_MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
  foreach(RINVID_MINGW_RUNTIME_DLL IN ITEMS libgcc_s_seh-1.dll libstdc++-6.dll
                                           libwinpthread-1.dll)
    if(EXISTS "${RINVID_MINGW_BIN_DIR}/${RINVID_MINGW_RUNTIME_DLL}")
      list(APPEND RINVID_MINGW_RUNTIME_DLLS
           "${RINVID_MINGW_BIN_DIR}/${RINVID_MINGW_RUNTIME_DLL}")
    endif()
  endforeach()
endif()

if(RINVID_PREVIOUS_BUILD_SHARED_LIBS_WAS_DEFINED)
  set(BUILD_SHARED_LIBS "${RINVID_PREVIOUS_BUILD_SHARED_LIBS}")
else()
  unset(BUILD_SHARED_LIBS)
endif()

if(RINVID_DEBUG_MODE_OUTPUT_TO_FILE AND NOT RINVID_DEBUG_MODE)
  message(WARNING "RINVID_DEBUG_MODE_OUTPUT_TO_FILE has no effect unless RINVID_DEBUG_MODE is ON")
endif()

function(rinvid_configure_target target)
  target_include_directories(
    ${target}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
           ${CMAKE_CURRENT_SOURCE_DIR}/extern/sfml/include
           ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
           ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
            ${FREETYPE_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb)
  if(RINVID_DEBUG_MODE)
    target_compile_definitions(${target} PUBLIC RINVID_DEBUG_MODE)
  endif()
  if(RINVID_DEBUG_MODE AND RINVID_DEBUG_MODE_OUTPUT_TO_FILE)
    target_compile_definitions(${target} PUBLIC RINVID_DEBUG_MODE_OUTPUT_TO_FILE)
  endif()
  target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -O3)
endfunction()

function(rinvid_copy_runtime_dependencies target)
  if(WIN32)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${target}>
              $<TARGET_FILE_DIR:${target}>
      COMMAND_EXPAND_LISTS)
    if(MINGW AND RINVID_MINGW_RUNTIME_DLLS)
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RINVID_MINGW_RUNTIME_DLLS}
                $<TARGET_FILE_DIR:${target}>
        COMMAND_EXPAND_LISTS)
    endif()
  endif()
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
if(UNIX)
  target_link_libraries(rinvid_core PUBLIC OpenGL::GL)
endif()

add_library(rinvid_platformers_obj OBJECT)
target_sources(rinvid_platformers_obj PRIVATE ${PLATFORMERS_SOURCES})
rinvid_configure_target(rinvid_platformers_obj)

add_library(rinvid_platformers STATIC $<TARGET_OBJECTS:rinvid_platformers_obj>)
rinvid_configure_target(rinvid_platformers)
target_link_libraries(rinvid_platformers PUBLIC rinvid_core rinvid_util)

add_library(rinvid_system_obj OBJECT)
target_sources(rinvid_system_obj PRIVATE ${SYSTEM_SOURCES})
rinvid_configure_target(rinvid_system_obj)
target_link_libraries(rinvid_system_obj PUBLIC SFML::Window)

add_library(rinvid_system STATIC $<TARGET_OBJECTS:rinvid_system_obj>)
rinvid_configure_target(rinvid_system)
target_link_libraries(rinvid_system PUBLIC rinvid_core rinvid_util SFML::Window)

add_library(rinvid_gui_obj OBJECT)
target_sources(rinvid_gui_obj PRIVATE ${GUI_SOURCES})
rinvid_configure_target(rinvid_gui_obj)
target_link_libraries(rinvid_gui_obj PUBLIC SFML::Window)

add_library(rinvid_gui STATIC $<TARGET_OBJECTS:rinvid_gui_obj>)
rinvid_configure_target(rinvid_gui)
target_link_libraries(rinvid_gui PUBLIC rinvid_core rinvid_system rinvid_util)

add_library(rinvid_sound_obj OBJECT)
target_sources(rinvid_sound_obj PRIVATE ${SOUND_SOURCES})
rinvid_configure_target(rinvid_sound_obj)
target_link_libraries(rinvid_sound_obj PUBLIC SFML::Audio)

add_library(rinvid_sound STATIC $<TARGET_OBJECTS:rinvid_sound_obj>)
rinvid_configure_target(rinvid_sound)
target_link_libraries(rinvid_sound PUBLIC SFML::Audio)

add_library(${PROJECT_NAME} STATIC $<TARGET_OBJECTS:rinvid_extern_obj>
                                   $<TARGET_OBJECTS:rinvid_util_obj>
                                   $<TARGET_OBJECTS:rinvid_core_obj>
                                   $<TARGET_OBJECTS:rinvid_platformers_obj>
                                   $<TARGET_OBJECTS:rinvid_system_obj>
                                   $<TARGET_OBJECTS:rinvid_gui_obj>
                                   $<TARGET_OBJECTS:rinvid_sound_obj>)
rinvid_configure_target(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC freetype SFML::Window SFML::Audio)
if(UNIX)
  target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
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
