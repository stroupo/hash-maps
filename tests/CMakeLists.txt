project(hashmap_tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests tests.cc)
target_link_libraries(tests PRIVATE catch)