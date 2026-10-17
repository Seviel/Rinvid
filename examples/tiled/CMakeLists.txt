add_executable(tiled main.cpp)
target_link_libraries(tiled PRIVATE rinvid)
target_compile_options(tiled PRIVATE -Werror -Wall -Wextra -pedantic -O3)
rinvid_copy_runtime_dependencies(tiled)

add_custom_command(
  TARGET tiled POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources
          $<TARGET_FILE_DIR:tiled>/resources)
