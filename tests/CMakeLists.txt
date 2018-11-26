cmake_minimum_required(VERSION 3.10)

if(NOT TARGET stroupo::hash_map)
  find_package(hash_map CONFIG REQUIRED)
endif()

add_executable(main main.cc)
target_link_libraries(main PRIVATE stroupo::hash_map)

enable_testing()
add_test(main main)