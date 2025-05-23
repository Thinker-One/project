aux_source_directory(src SRC)
add_executable(asio ${SRC})
#target_compile_definitions(asio PUBLIC ASIO_STANDALONE)
target_include_directories(asio PRIVATE include lib/boost)