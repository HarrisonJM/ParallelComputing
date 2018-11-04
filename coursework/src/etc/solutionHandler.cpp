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

    _readySolutions = new std::queue<std::string>[tempNum];
}
/*!
 * @brief Destructor
 */
SolutionHandler::~SolutionHandler()
{
}
/*!
 * @brief generates solutiosn based on the threadhandler
 */
void SolutionHandler::GenUsingHandler()
{
    while (!_th.getDone())
    {
        _CreateSolutions();
    }
}
/*!
 * @brief Just generates a bunch of soultions
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
const std::string SolutionHandler::getNextSolution(const int solverID)
{
    // outOfBounds
    if (solverID > _numberOfThreads)
        return "";

    const char *solution = _readySolutions[solverID].front().c_str();
    _readySolutions[solverID].pop();

    return solution;
}
/*!
 * @brief Returns a pointer to the queue
 * @param solverID The ID fo the solver linked to the queue
 * @return A pointer to the solvers queue
 */
std::queue<std::string> *SolutionHandler::getQueue(const int solverID)
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
    for (int i = 0; i < _numberOfThreads; ++i)
    {
        if (_readySolutions[i].size() < 100)
        {
            _readySolutions[i].push(_keyGenerator.getString());
            _keyGenerator.incrementString();
        }
    }
}
} /* NAMESPACE etc::solutionHandler */