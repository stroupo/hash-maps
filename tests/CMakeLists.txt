cmake_minimum_required(VERSION 3.10)
enable_testing()

find_package(doctest REQUIRED)

# this makes internal and external testing possible
if(NOT TARGET stroupo::hash_map_lib)
  find_package(hash_map_lib CONFIG REQUIRED)
endif()

add_executable(main_test
  doctest_main.cc
  hash_map.cc
  ranges.cc
)

target_link_libraries(main_test
  PRIVATE
    doctest::doctest
    stroupo::hash_map_lib
)

add_test(main_test main_test)