cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hash-map)

add_subdirectory(thirdparty/catch)
add_subdirectory(src)
add_subdirectory(tests)
