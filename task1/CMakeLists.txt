cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(task1)

option(float "Build the float arr" OFF)

if (float)
	add_executable(task1 task1.cpp)
	add_definitions(-DUse_float)
else()
	add_executable(task1 task1.cpp)
endif()