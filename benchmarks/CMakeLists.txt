if(NOT EXISTS "${RINVID_BENCHMARK_SOURCE_DIR}/CMakeLists.txt")
  message(
    FATAL_ERROR
      "Google Benchmark source not found.\n"
      "Expected a checkout at: ${RINVID_BENCHMARK_SOURCE_DIR}\n"
      "Clone or unpack benchmark there, or configure with:\n"
      "  -DRINVID_BENCHMARK_SOURCE_DIR=/absolute/path/to/benchmark")
endif()

set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
add_subdirectory("${RINVID_BENCHMARK_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/google_benchmark"
                 EXCLUDE_FROM_ALL)

file(GLOB_RECURSE BENCHMARK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(rinvid_benchmark ${BENCHMARK_SOURCES})

target_link_libraries(rinvid_benchmark PRIVATE rinvid perf_stress_common benchmark::benchmark_main)
target_compile_options(rinvid_benchmark PRIVATE -Werror -Wall -Wextra -pedantic -O3)

set(BENCHMARK_FONT_RESOURCE ${PROJECT_SOURCE_DIR}/examples/testing_grounds/resources/aquifer.ttf)
add_custom_command(TARGET rinvid_benchmark POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                                                             make_directory
                                                             $<TARGET_FILE_DIR:rinvid_benchmark>/resources)
add_custom_command(TARGET rinvid_benchmark POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                                                             copy_if_different
                                                             ${BENCHMARK_FONT_RESOURCE}
                                                             $<TARGET_FILE_DIR:rinvid_benchmark>/resources/aquifer.ttf)
add_custom_command(TARGET rinvid_benchmark POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
                                                             ${PROJECT_BINARY_DIR}/resources)
add_custom_command(TARGET rinvid_benchmark POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                                                             copy_if_different
                                                             ${BENCHMARK_FONT_RESOURCE}
                                                             ${PROJECT_BINARY_DIR}/resources/aquifer.ttf)
