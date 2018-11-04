
add_library(key
        keySegment.cpp
        key.cpp)

#############################
add_library(decipher
        decipherAgentHandler.cpp
        decipherAgent.cpp)
target_link_libraries(decipher
        threadHandler)


#############################
add_library(threadHandler
        threadHandler.cpp)


#############################
add_library(solutionHandler
        solutionHandler.cpp)
target_link_libraries(solutionHandler
        key
        threadHandler)