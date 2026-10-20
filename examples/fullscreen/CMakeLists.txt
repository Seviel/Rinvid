add_executable(fullscreen WIN32 main.cpp)
target_link_libraries(fullscreen PRIVATE rinvid)
target_compile_options(fullscreen PRIVATE -Werror -Wall -Wextra -pedantic -O3)
rinvid_copy_runtime_dependencies(fullscreen)
add_custom_command(TARGET fullscreen POST_BUILD COMMAND ${CMAKE_COMMAND}
  -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources
  $<TARGET_FILE_DIR:fullscreen>/resources)
