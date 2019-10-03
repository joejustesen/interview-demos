cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project(quick-base-demo VERSION 0.1 LANGUAGES CXX)

add_executable(demo
  CPPCraftDemo.cpp
  InMemoryIndex.cpp)

set_property(TARGET demo PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -O3 -std=c++17")
