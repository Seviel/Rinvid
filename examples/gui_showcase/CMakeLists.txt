file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(gui_showcase WIN32 ${EXAMPLE_SOURCES})

target_link_libraries(gui_showcase PRIVATE rinvid)
target_compile_options(gui_showcase PRIVATE -Werror -Wall -Wextra -pedantic -O3)
rinvid_copy_runtime_dependencies(gui_showcase)

# Copy resources to build directory so the executable can find them
set(EXAMPLE_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_custom_command(
  TARGET gui_showcase
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${EXAMPLE_RESOURCES_DIR}
          $<TARGET_FILE_DIR:gui_showcase>/resources)
