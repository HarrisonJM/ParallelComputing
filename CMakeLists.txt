cmake_minimum_required(VERSION 3.9.6)
## Project must go here #
project(ProtocolDeveloper LANGUAGES C CXX VERSION 0.1 )
set(CMAKE_C_FLAGS "-std=c99 -lpthread -ldl -Wall -fopenmp")
set(CMAKE_CXX_FLAGS "-std=c++17 -lpthread -ldl -Wall -fopenmp")

find_package(OpenCL)
if (OPENCL_FOUND)
    include_directories(${OpenCL_INCLUDE_DIRS})
    link_directories(${OpenCL_LIBRARY})
endif()


include_directories(include)

add_subdirectory(week1)
add_subdirectory(week2)
add_subdirectory(week3)
add_subdirectory(week4)
add_subdirectory(week5)

#add_subdirectory(coursework)