/*!
 * @brief Will be used by all the threads to track when they are done
 *
 * @date 03/11/18
 */


#ifndef PROTOCOLDEVELOPER_THREADHANDLER_H
#define PROTOCOLDEVELOPER_THREADHANDLER_H

#include <omp.h>

namespace etc::threadHandler
{

class ThreadHandler
{
public:
    ThreadHandler(omp_lock_t &lock);
    ~ThreadHandler() = default;

    bool getDone();
    void setDone();
private:
    bool _done;
    omp_lock_t &_lock;
};
} /* NAMESPACE etc::threadHandler */

#endif /*PROTOCOLDEVELOPER_THREADHANDLER_H*/
