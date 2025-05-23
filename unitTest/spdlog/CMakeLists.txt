aux_source_directory(src SRC)
add_executable(spdlog ${SRC})
target_include_directories(spdlog PRIVATE include)
