# Top-level CMake configuration for Capstone project
cmake_minimum_required(VERSION 3.16)

# Set CMake policies to the latest for forward compatibility
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project("Capstone")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable IDE folder structure
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add main project subdirectory
add_subdirectory("RoboDad")
