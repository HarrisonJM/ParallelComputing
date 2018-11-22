/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 03/11/18
 */



#include <sstream>
#include "solutionHandler.h"
#include "threadHandler.h"

namespace etc::solutionHandler
{

/*!
 * @brief Cosntructor, takes number of threads and a threadhandler
 * @param numberofThreads The maximum number of threads
 * @param thread The threadhandler we'll be using
 */
SolutionHandler::SolutionHandler(const int numberofThreads
                                 , threadHandler::ThreadHandler &thread)
    : _numberOfThreads(numberofThreads)
      , _readySolutions()
      , _th(thread)
      , _keyGenerator()
{
    _keyGenerator = etc::key::key();
    int tempNum = 1;

    if (numberofThreads > 0)
        tempNum = _numberOfThreads;

    _readySolutions = new std::queue<const uint8_t*>[tempNum];
}
/*!
 * @brief generates solutiosn based on the threadhandler
 */
void SolutionHandler::GenUsingHandler()
{
    bool done = false;

    do
    {
        _CreateSolutions();

//        omp_set_lock(_lock);
        done = _th.getDone();
//        omp_set_lock(_lock);
    } while (!done);

}
/*!
 * @brief Just generates a bunch of solutions
 */
void SolutionHandler::Gen()
{
    _CreateSolutions();
}
/*!
 * @brief Returns a solution corresponding to
 * @param solverID The ID of the solver to grab a solution
 * @return A char* containing the solution
 */
const uint8_t * SolutionHandler::getNextSolution(const int solverID)
{
    // outOfBounds
    if (solverID > _numberOfThreads)
        return (uint8_t*)"0000000000000000";

    const uint8_t *solution = _readySolutions[solverID].front();
    _readySolutions[solverID].pop();

    return solution;
}
/*!
 * @brief Returns a pointer to the queue
 * @param solverID The ID fo the solver linked to the queue
 * @return A pointer to the solvers queue
 */
std::queue<const uint8_t *> * SolutionHandler::getQueue(const int solverID)
{
    if (solverID > _numberOfThreads)
        return nullptr;

    return (_readySolutions + solverID);
}
/*!
 * @brief Generates solutions
 */
void SolutionHandler::_CreateSolutions()
{
    int tempNum = 1;

    if(_numberOfThreads > 0)
        tempNum = _numberOfThreads;

    for (int i = 0; i < tempNum; ++i)
    {
        if (_readySolutions[i].size() < 100)
        {
            _readySolutions[i].push(_keyGenerator.getStringNorm());

            _keyGenerator.incrementStringNorm();
        }
    }
}
/*!
 * @brief grabs a single key
 * @return A pointer holding the string
 */
const uint8_t * SolutionHandler::GetOneSolution()
{
    static etc::key::key key1;
    key1.incrementStringNorm();

    const uint8_t* key = key1.getStringNorm();

    return key;
}
} /* NAMESPACE etc::solutionHandler */