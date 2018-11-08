#ifndef _OPENMP
assert(1, "NO OPENMP!");
#endif

#include <omp.h>
#include <iostream>

#include "key/key.h"
#include "threadHandler.h"
#include "solutionHandler.h"
#include "ssl/decipherAgent.h"

int main(void)
{
    etc::key::key key;
    omp_lock_t threadLock;

    etc::threadHandler::ThreadHandler th(threadLock);

    etc::solutionHandler::SolutionHandler sg(1 ,th);
    etc::decipher::decipherAgent(0, th,
    )

    return 0;
}