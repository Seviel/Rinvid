if(NOT EXISTS "${RINVID_GTEST_SOURCE_DIR}/CMakeLists.txt")
  message(
    FATAL_ERROR
      "GoogleTest source not found.\n"
      "Expected a checkout at: ${RINVID_GTEST_SOURCE_DIR}\n"
      "Clone or unpack googletest there, or configure with:\n"
      "  -DRINVID_GTEST_SOURCE_DIR=/absolute/path/to/googletest")
endif()

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory("${RINVID_GTEST_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/googletest"
                 EXCLUDE_FROM_ALL)

file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(rinvid_test ${EXAMPLE_SOURCES})

target_include_directories(rinvid_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(rinvid_test PRIVATE rinvid gtest)
target_compile_options(rinvid_test PRIVATE -Werror -Wall -Wextra -pedantic
                                               -O3)

add_test(NAME rinvid_test COMMAND ./$<TARGET_FILE_NAME:rinvid_test>)

# Copy resources to build directory so the executable can find them
set(EXAMPLE_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_custom_command(TARGET rinvid_test POST_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory ${EXAMPLE_RESOURCES_DIR}
$<TARGET_FILE_DIR:rinvid_test>/resources )
add_custom_command(TARGET rinvid_test POST_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory ${EXAMPLE_RESOURCES_DIR}
${PROJECT_BINARY_DIR}/resources )
add_custom_command(TARGET rinvid_test POST_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory ${EXAMPLE_RESOURCES_DIR}
$<TARGET_FILE_DIR:rinvid_test>/tests/resources )
