#include <courseworkHandler.h>

#ifndef _OPENMP
assert(1, "NO OPENMP!");
#endif

int main()
{
    etc::CourseworkHandler ch;
    ch.StartSerial();

    return 0;
}