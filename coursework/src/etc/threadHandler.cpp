/*!
 * @brief Used by the threads to get/set done
 *
 * @date 03/11/18
 */

#include <omp.h>

#include "threadHandler.h"

namespace etc::threadHandler
{

ThreadHandler::ThreadHandler()
    : _done(false)
{
}

bool ThreadHandler::getDone()
{
    return _done;
}

void ThreadHandler::setDone(bool update)
{
    _done = true;
}
} /* NAMESPACE etc::threadHandler */