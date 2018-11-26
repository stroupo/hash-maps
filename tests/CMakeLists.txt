cmake_minimum_required(VERSION 3.10)

if(NOT TARGET stroupo::hash_map)
  find_package(hash_map CONFIG REQUIRED)
endif()

add_executable(package_test main.cc)
target_link_libraries(package_test PRIVATE stroupo::hash_map)

enable_testing()
add_test(package_test package_test)