cmake_minimum_required(VERSION 3.9.6)
## Project must go here #
project(ProtocolDeveloper LANGUAGES C CXX VERSION 0.1 )
set(CMAKE_CXX_FLAGS "-std=c++17 -lpthread -Wall -fopenmp")

include(GNUInstallDirs)
include_directories(src/etc)

add_subdirectory(src)