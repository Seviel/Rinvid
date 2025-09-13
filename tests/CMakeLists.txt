file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(rinvid_test WIN32 ${EXAMPLE_SOURCES})

target_include_directories(rinvid_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(rinvid_test PRIVATE rinvid
                                        gtest)
target_compile_options(rinvid_test PRIVATE -Werror -Wall -Wextra -pedantic
                                               -O3)

# Copy resources to build directory so the executable can find them
set(EXAMPLE_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_custom_command(TARGET rinvid_test POST_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory ${EXAMPLE_RESOURCES_DIR}
$<TARGET_FILE_DIR:rinvid_test>/resources )
