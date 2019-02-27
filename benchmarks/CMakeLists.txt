cmake_minimum_required(VERSION 3.5)

find_package(Boost COMPONENTS filesystem REQUIRED)

# this makes internal and external testing possible
if(NOT TARGET stroupo::hash_map_lib)
  find_package(hash_map_lib CONFIG REQUIRED)
endif()

add_executable(bench
  bench.cc
)

target_link_libraries(bench
  PRIVATE
    stroupo::hash_map_lib
    Boost::filesystem
)
