cmake_minimum_required(VERSION 3.11)
project(hash_map VERSION 0.1.0 LANGUAGES CXX)

add_library(hash_map INTERFACE)
target_include_directories(hash_map INTERFACE .)
target_compile_features(hash_map INTERFACE cxx_std_17)

add_library(stroupo::hash_map ALIAS hash_map)

enable_testing()
add_subdirectory(tests)