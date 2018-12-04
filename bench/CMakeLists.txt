cmake_minimum_required(VERSION 3.11)

find_package(Boost COMPONENTS system filesystem REQUIRED)
project(hash_map_benchmarks VERSION 0.1.0 LANGUAGES CXX)


add_executable(bench bench.cc)
target_link_libraries(bench PRIVATE stroupo::hash_map)
target_link_libraries(bench PRIVATE 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)