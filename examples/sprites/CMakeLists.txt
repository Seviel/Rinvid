file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(sprites WIN32 ${EXAMPLE_SOURCES})

target_include_directories(sprites PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(sprites PRIVATE rinvid)
target_compile_options(sprites PRIVATE -Werror -Wall -Wextra -pedantic
                                               -O3)

# Copy resources to build directory so the executable can find them
set(EXAMPLE_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_custom_command(TARGET sprites POST_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory ${EXAMPLE_RESOURCES_DIR}
$<TARGET_FILE_DIR:sprites>/resources )
