/*!
 * @brief Generates individual solutions, relies on threads not completing at the same time
 *
 * @date 03/11/18
 */

#include <string>
#include <vector>
#include <queue>
#include "threadHandler.h"
#include "key.h"

#ifndef PROTOCOLDEVELOPER_SOLUTIONGENERATOR_H
#define PROTOCOLDEVELOPER_SOLUTIONGENERATOR_H

namespace etc::solutionHandler
{

class SolutionHandler
{
public:
    SolutionHandler(const int numberOfThreads
                    , threadHandler::ThreadHandler &thread);
    ~SolutionHandler() = default;

    void GenUsingHandler();
    void Gen();

    const uint8_t * getNextSolution(const int solverID);
    std::queue<const uint8_t *> * getQueue(const int solverID);

private:
    /* members */
    const int _numberOfThreads;
    //! Holds all the things for the stuffs
    std::queue<const uint8_t*> *_readySolutions;
    threadHandler::ThreadHandler &_th;

    etc::key::key _keyGenerator;

    /* methods */
    void _CreateSolutions();
};

} /* NAMESPACE etc::solutionHandler */

#endif /*PROTOCOLDEVELOPER_SOLUTIONGENERATOR_H*/
