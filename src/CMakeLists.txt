cmake_minimum_required(VERSION 3.11)

project(hashmap)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_executable(hashmap main.cc)
add_library(hash_map INTERFACE)
target_include_directories(hash_map INTERFACE .)

add_library(stroupo::hash_map ALIAS hash_map)
add_executable(main main.cc)
target_link_libraries(main PRIVATE stroupo::hash_map)