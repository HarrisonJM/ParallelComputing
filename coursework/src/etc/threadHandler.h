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
    ThreadHandler();
    ~ThreadHandler() = default;

    bool getDone();
    void setDone(bool update);
private:
    bool _done;
};
} /* NAMESPACE etc::threadHandler */

#endif /*PROTOCOLDEVELOPER_THREADHANDLER_H*/
