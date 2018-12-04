cmake_minimum_required(VERSION 3.11)
project(hash_map_benchmarks VERSION 0.1.0 LANGUAGES CXX)


add_executable(bench bench2.cc)
target_link_libraries(bench PRIVATE stroupo::hash_map)