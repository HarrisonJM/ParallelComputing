cmake_minimum_required(VERSION 3.9.6)
## Project must go here #
project(ProtocolDeveloper LANGUAGES C CXX VERSION 0.1)

set(CMAKE_CXX_FLAGS "-std=c++17 -lpthread -Wall -fopenmp")
set(CMAKE_CXX_COMPILER mpic++)

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif ()

include(GNUInstallDirs)
include_directories(src/etc)

add_library(key
        src/etc/key/key.cpp)

#############################
add_library(cipherBoy
        src/etc/ssl/cipherDoer.cpp
        src/etc/ssl/cipherDoer.h)

#############################

add_library(courseworkHandler
        src/etc/courseworkHandler.cpp
        src/etc/courseworkHandler.h)

target_link_libraries(courseworkHandler
        cipherBoy
        key
        OpenSSL::SSL)

add_executable(coursework.elf
        src/binLib/pc1.cpp)

target_link_libraries(coursework.elf
        courseworkHandler)