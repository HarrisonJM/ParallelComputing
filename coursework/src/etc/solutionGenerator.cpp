/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 03/11/18
 */



#include <sstream>
#include "SolutionGenerator.h"

namespace etc
{

SolutionGenerator::SolutionGenerator(const int numberofThreads)
    : _numberOfThreads(numberofThreads),
      _readySolutions(_numberOfThreads)
{
}

SolutionGenerator::~SolutionGenerator()
{
}

/*!
 * @brief returns a solution corresponding to
 * @param solverID The ID of the solver to grab a solution
 * @return A char* containing the solution
 */
const char* SolutionGenerator::getNextSolution(const int solverID)
{
    // outOfBounds
    if(solverID > _numberOfThreads)
        return "";

    const char* solution = _readySolutions[solverID].front().c_str();
    _readySolutions[solverID].pop();

    return solution;
}
/*!
 * @brief Generates solutions
 */
void SolutionGenerator::_GenerateSolutions()
{
    std::stringstream lastSolution("");
    int counter = 0;
    // keep 250 solutions backed up
    const int maxSize = 250;
    int totalSize = 0;


}

} /* etc */