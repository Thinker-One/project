aux_source_directory(src SRC)
add_executable(nlohmann ${SRC})
target_include_directories(nlohmann PRIVATE include)