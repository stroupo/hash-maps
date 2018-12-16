cmake_minimum_required(VERSION 3.5)

find_package(Boost COMPONENTS filesystem REQUIRED)

project(hash_map_benchmarks VERSION 0.1.0 LANGUAGES CXX)


add_executable(bench bench.cc)
target_link_libraries(bench PRIVATE stroupo::hash_map)
target_link_libraries(bench PRIVATE Boost::filesystem)