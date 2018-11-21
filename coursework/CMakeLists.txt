cmake_minimum_required(VERSION 3.9.6)
## Project must go here #
project(ProtocolDeveloper LANGUAGES C CXX VERSION 0.1)

set(CMAKE_CXX_FLAGS "-std=c++17 -lpthread -Wall -fopenmp")

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

include(GNUInstallDirs)
include_directories(src/etc)

add_library(key
        src/etc/key/keySegment.cpp
        src/etc/key/key.cpp)

#############################
add_library(threadHandler
        src/etc/threadHandler.cpp
        src/etc/threadHandler.h)

#############################
add_library(decipher
        src/etc/ssl/cipherAgent.cpp
        src/etc/ssl/cipherAgent.h)

target_link_libraries(decipher
        threadHandler)

#############################
add_library(solutionHandler
        src/etc/solutionHandler.cpp
        src/etc/solutionHandler.h)

target_link_libraries(solutionHandler
        key
        threadHandler)

#############################

add_library(courseworkHandler
        src/etc/courseworkHandler.cpp
        src/etc/courseworkHandler.h)

target_link_libraries(courseworkHandler
        threadHandler
        solutionHandler
        decipher
        key
        OpenSSL::SSL)

add_executable(coursework.elf
        src/binLib/pc1.cpp)

target_link_libraries(coursework.elf
        courseworkHandler)