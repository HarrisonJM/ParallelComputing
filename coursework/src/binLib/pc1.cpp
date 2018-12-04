#include "../etc/courseworkHandler.h"

#include <omp.h>
#include <iostream>

int main()
{
    etc::CourseworkHandler ch;

    double startSerial = omp_get_wtime();
    ch.StartSerial();
    double finishSerial = omp_get_wtime();

    double startOMP = omp_get_wtime();
    ch.StartOpenMP();
    double finishOMP = omp_get_wtime();

#ifdef _USE_MPI_
    double startMPI = omp_get_wtime();
    ch.StartMPI();
    double finishMPI = omp_get_wtime();
#endif /* _USE_MPI_ */

    std::cout <<
              "Serial Time: " << (finishSerial-startSerial) <<
              "   OMP Time: " << (finishOMP-startOMP) <<
              #ifdef _USE_MPI_
    "   MPI Time: " << (finishMPI-startMPI) <<
              #endif
              std::endl;

    return 0;
}