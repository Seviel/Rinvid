set(PERF_STRESS_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/stress_scene.cpp")

add_library(perf_stress_common STATIC ${PERF_STRESS_COMMON_SOURCES})
target_include_directories(perf_stress_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(perf_stress_common PUBLIC rinvid)
target_compile_options(perf_stress_common PRIVATE -Werror -Wall -Wextra -pedantic -O3)

set(PERF_STRESS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/perf_stress_screen.cpp")

add_executable(perf_stress WIN32 ${PERF_STRESS_SOURCES})

target_link_libraries(perf_stress PRIVATE perf_stress_common rinvid)
target_compile_options(perf_stress PRIVATE -Werror -Wall -Wextra -pedantic -O3)
rinvid_copy_runtime_dependencies(perf_stress)

set(PERF_STRESS_FONT_RESOURCE
    ${PROJECT_SOURCE_DIR}/examples/testing_grounds/resources/aquifer.ttf)
add_custom_command(TARGET perf_stress POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
                                                             $<TARGET_FILE_DIR:perf_stress>/resources)
add_custom_command(TARGET perf_stress POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                                                             copy_if_different
                                                             ${PERF_STRESS_FONT_RESOURCE}
                                                             $<TARGET_FILE_DIR:perf_stress>/resources/aquifer.ttf)
add_custom_command(TARGET perf_stress POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
                                                             ${PROJECT_BINARY_DIR}/resources)
add_custom_command(TARGET perf_stress POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                                                             copy_if_different
                                                             ${PERF_STRESS_FONT_RESOURCE}
                                                             ${PROJECT_BINARY_DIR}/resources/aquifer.ttf)
