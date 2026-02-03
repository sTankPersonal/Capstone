# Top-level CMake configuration for Capstone project
cmake_minimum_required(VERSION 3.16)
project(Capstone VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    include(CTest)
endif()

# Export compile commands for IDE IntelliSense (supported by Ninja generator)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Option to control whether build warnings are shown. When OFF, add
# compiler flags to suppress all warnings and show only errors.
option(SHOW_BUILD_WARNINGS "Show compiler warnings during build" OFF)
if(NOT SHOW_BUILD_WARNINGS)
	if(MSVC)
		add_compile_options(/W0)
	else()
		add_compile_options(-w)
	endif()
endif()
