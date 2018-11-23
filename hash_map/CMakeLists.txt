cmake_minimum_required(VERSION 3.11)
project(hash_map VERSION 0.1.0 LANGUAGES CXX)

# Build system
add_library(hash_map INTERFACE)
# set_target_properties(hash_map
#   PROPERTIES
#     PUBLIC_HEADER hash_map.h
# )
target_include_directories(hash_map
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)
target_compile_features(hash_map
  INTERFACE
    cxx_std_17
)
add_library(stroupo::hash_map ALIAS hash_map)

# Testing
enable_testing()
add_subdirectory(tests)

# Package installation
install(
  TARGETS hash_map
  EXPORT hash_mapTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  # PUBLIC_HEADER DESTINATION include/stroupo/hash_map
)
install(
  FILES
    hash_map.h
  DESTINATION
    include/hash_map
)

install(
  EXPORT hash_mapTargets
  NAMESPACE stroupo::
  DESTINATION lib/cmake/hash_map
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  hash_mapConfigVersion.cmake
  VERSION 0.1.0
  COMPATIBILITY SameMajorVersion
)
install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/hash_mapConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/hash_mapConfigVersion.cmake
  DESTINATION
    lib/cmake/hash_map
)

