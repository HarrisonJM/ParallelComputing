add_executable(pcExe
        pc1.cpp)

target_link_libraries(pcExe
        key
        solutionHandler
        threadHandler
        decipher)