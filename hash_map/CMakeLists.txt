cmake_minimum_required(VERSION 3.11)

project(hash_map
  VERSION 0.1.0
  LANGUAGES CXX
  DESCRIPTION
    "A stroupo library with different implementations for hash maps."
)
set(PROJECT_NAMESPACE "stroupo")

# Configuration
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Building
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)
target_compile_features(${PROJECT_NAME}
  INTERFACE cxx_std_17
)
add_library(${PROJECT_NAMESPACE}::${PROJECT_NAME}
  ALIAS ${PROJECT_NAME}
)

# Testing
add_subdirectory(tests)

# Deploying
include(${PROJECT_NAME}Install)
include(${PROJECT_NAME}PkgconfigInstall)
include(${PROJECT_NAME}Uninstall)
include(${PROJECT_NAME}Export)