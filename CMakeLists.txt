cmake_minimum_required(VERSION 3.4)

project(MPL_QuickSort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)

add_executable(quick_sort main_quicksort.cpp)

add_executable(select_sort main_select.cpp)

add_executable(heap_sort main_heap.cpp)
