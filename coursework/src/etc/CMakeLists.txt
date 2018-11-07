
add_library(key
        keySegment.cpp
        key.cpp)

#############################
add_library(threadHandler
        threadHandler.cpp
        threadHandler.h)

#############################
add_library(decipher
        decipherAgentHandler.cpp
        decipherAgentHandler.h
        decipherAgent.cpp
        decipherAgent.h courseworkHandler.cpp courseworkHandler.h)

target_link_libraries(decipher
        threadHandler)

#############################
add_library(solutionHandler
        solutionHandler.cpp
        solutionHandler.h)

target_link_libraries(solutionHandler
        key
        threadHandler)