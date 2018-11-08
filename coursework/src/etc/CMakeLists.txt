
add_library(key
        key/keySegment.cpp
        key/key.cpp)

#############################
add_library(threadHandler
        threadHandler.cpp
        threadHandler.h)

#############################
add_library(decipher
        ssl/decipherAgentHandler.cpp
        ssl/decipherAgentHandler.h
        ssl/decipherAgent.cpp
        ssl/decipherAgent.h courseworkHandler.cpp courseworkHandler.h)

target_link_libraries(decipher
        threadHandler)

#############################
add_library(solutionHandler
        solutionHandler.cpp
        solutionHandler.h)

target_link_libraries(solutionHandler
        key
        threadHandler)

############################
add_library(textReader
        readerWriter/encipheredTextReader.cpp)