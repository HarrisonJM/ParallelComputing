#ifndef _OPENMP
assert(1, "NO OPENMP!");
#endif

#include <omp.h>
#include <iostream>

#include "key.h"
#include "threadHandler.h"
#include "solutionHandler.h"

int main(void)
{
    etc::key::key key;
    omp_lock_t threadLock;

    etc::threadHandler::ThreadHandler th(threadLock);

    etc::solutionHandler::SolutionHandler sg(1 ,th);

//    sg.Gen();
//    std::queue<std::string>* sq = sg.getQueue(0);

//    std::string foo = sq->front();
//    std::cout << foo << std::endl;

    return 0;
}