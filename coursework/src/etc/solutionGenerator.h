/*!
 * @brief Generates individual solutions, relies on threads not completing at the same time
 *
 * @date 03/11/18
 */

#include <string>
#include <vector>
#include <queue>

#ifndef PROTOCOLDEVELOPER_SOLUTIONGENERATOR_H
#define PROTOCOLDEVELOPER_SOLUTIONGENERATOR_H

namespace etc
{

class SolutionGenerator
{
public:
    SolutionGenerator(const int numberOfThreads);
    ~SolutionGenerator();

    const char * getNextSolution(const int solverID);
private:
    /* members */
    const int _numberOfThreads;
    //! Holds all the things for the stuffs
    std::vector<std::queue<std::string>> _readySolutions;

    /* methods */
    void _GenerateSolutions();
};

} /* NAMESPACE etc*/

#endif /*PROTOCOLDEVELOPER_SOLUTIONGENERATOR_H*/
