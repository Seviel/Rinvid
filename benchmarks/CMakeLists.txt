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

target_link_libraries(rinvid_benchmark PRIVATE rinvid benchmark::benchmark_main)
target_compile_options(rinvid_benchmark PRIVATE -Werror -Wall -Wextra -pedantic -O3)
