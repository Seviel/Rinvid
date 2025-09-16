file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(full_demo WIN32 ${EXAMPLE_SOURCES})

target_include_directories(full_demo PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(full_demo PRIVATE rinvid)
target_compile_options(full_demo PRIVATE -Werror -Wall -Wextra -pedantic -O3)

# Copy resources to build directory so the executable can find them
set(EXAMPLE_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_custom_command(
  TARGET full_demo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${EXAMPLE_RESOURCES_DIR}
          $<TARGET_FILE_DIR:full_demo>/resources)
