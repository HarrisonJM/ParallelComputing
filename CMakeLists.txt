cmake_minimum_required(VERSION 3.9.6)
## Project must go here #
project(ProtocolDeveloper LANGUAGES C CXX VERSION 0.1 )
set(CMAKE_CXX_FLAGS "-std=gnu++17 -std=c++17 -lpthread -ldl -Wall")

include_directories(include)

add_subdirectory(coursework)
add_subdirectory(week1)
add_subdirectory(week2)
add_subdirectory(week3)