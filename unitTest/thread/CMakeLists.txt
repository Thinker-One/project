aux_source_directory(src SRC)
add_executable(thread ${SRC})
target_include_directories(thread PRIVATE include)