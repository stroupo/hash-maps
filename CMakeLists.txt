cmake_minimum_required(VERSION 3.10)
project(stroupo-hash-maps VERSION 0.1.0 LANGUAGES CXX)

add_library(hash_map INTERFACE)
target_compile_features(hash_map INTERFACE cxx_std_17)
target_include_directories(hash_map INTERFACE .)
add_library(stroupo::hash_map ALIAS hash_map)

add_executable(main main.cc)
target_link_libraries(main PRIVATE stroupo::hash_map)