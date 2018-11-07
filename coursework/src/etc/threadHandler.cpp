/*!
 * @brief Used by the threads to get/set done
 *
 * @date 03/11/18
 */

#include <omp.h>

#include "threadHandler.h"

namespace etc::threadHandler
{

ThreadHandler::ThreadHandler(omp_lock_t &lock)
    : _done(false)
      , _lock(lock)
{
}

bool ThreadHandler::getDone()
{
    omp_set_lock(&_lock);
    return _done;
    omp_unset_lock(&_lock);
}

void ThreadHandler::setDone(bool update)
{
    omp_set_lock(&_lock);
    _done = true;
    omp_unset_lock(&_lock);
}
} /* NAMESPACE etc::threadHandler */