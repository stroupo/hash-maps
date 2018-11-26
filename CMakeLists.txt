cmake_minimum_required(VERSION 3.11)
project(stroupo-hash-maps VERSION 0.1.0 LANGUAGES CXX)

enable_testing()

add_subdirectory(hash_map)

add_subdirectory(tests)

# add_executable(main main.cc)
# target_link_libraries(main PRIVATE stroupo::hash_map)
