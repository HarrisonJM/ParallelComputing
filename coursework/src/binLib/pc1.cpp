#include "../etc/courseworkHandler.h"

#include <omp.h>
#include <iostream>

int main()
{
    etc::CourseworkHandler ch;

#ifndef _USE_MPI_
    double startSerial = omp_get_wtime();
    ch.StartSerial();
    double finishSerial = omp_get_wtime();
#endif /* _USE_MPI_ */

#ifdef _USE_OPEN_MP_
    double startOMP = omp_get_wtime();
    ch.StartOpenMP();
    double finishOMP = omp_get_wtime();
#endif /* _USE_OPEN_MP_ */

#ifdef _USE_MPI_
    double startMPI = omp_get_wtime();
    ch.StartMPI();
    double finishMPI = omp_get_wtime();
#endif /* _USE_MPI_ */

    std::cout <<
              #ifndef _USE_MPI_
              "Serial Time: " << (finishSerial-startSerial) <<
              #endif /*  _USE_MPI_ */
              #ifdef _USE_OPEN_MP_
              "   OMP Time: " << (finishOMP-startOMP) <<
              #endif /* _USE_OPEN_MP_ */
              #ifdef _USE_MPI_
              "   MPI Time: " << (finishMPI-startMPI) <<
              #endif /* _USE_MPI_ */
              std::endl;

    return 0;
}
