cmake_minimum_required(VERSION 3.10)
enable_testing()

find_package(doctest REQUIRED)
if(NOT TARGET stroupo::hash_map)
  find_package(hash_map CONFIG REQUIRED)
endif()

add_executable(main_test
  doctest_main.cc
  hash_map.cc
  ranges.cc
)

target_link_libraries(main_test
  PRIVATE
    doctest::doctest
    stroupo::hash_map
)

target_compile_options(main_test
  PRIVATE
    -g
)

add_test(hash_map_test
  main_test
)

add_test(coverage
  kcov --include-path=${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/kcov main_test
)

# add_test(ranges_test
#   main_test
# )