# Andrew Black
# May 27, 2017

cmake_minimum_required(VERSION 3.1)
project(HeterogenousMap)

add_library(HeterogenousMap INTERFACE)
target_include_directories(HeterogenousMap INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	)
target_sources(HeterogenousMap INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ablack94/hmap/HMap.hpp"
	)

# Show files in IDE - not sure if there's a better way to do this
# https://stackoverflow.com/questions/27039019/how-to-have-cmake-show-headers-that-are-not-part-of-any-binary-target-in-the-ide
add_custom_target(HeterogenousMap_ide SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ablack94/hmap/HMap.hpp"
	)


