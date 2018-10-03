cmake_minimum_required(VERSION 3.9.6)
## Project must go here #
project(ProtocolDeveloper LANGUAGES CXX VERSION 0.1 )
set(CMAKE_CXX_FLAGS "-std=gnu++17 -std=c++17 -lpthread -ldl -Wall")


add_subdirectory(coursework)
add_subdirectory(week1)