file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(primitive_shapes WIN32 ${EXAMPLE_SOURCES})

target_include_directories(primitive_shapes PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(primitive_shapes PRIVATE rinvid)
target_compile_options(primitive_shapes PRIVATE -Werror -Wall -Wextra -pedantic
                                               -O3)
