# Top-level CMake configuration for Capstone project
cmake_minimum_required(VERSION 3.16)
project(Capstone VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build unit tests" ON)

function(enable_recommended_warnings target)
	target_compile_options(${target} PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
		$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
	)
endfunction()

# Enable IDE folder structure
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add main project subdirectory
add_subdirectory("RoboDad")
