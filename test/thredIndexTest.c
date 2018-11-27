#include <omp.h>
#include <stdio.h>

int main()
{
#pragma omp parallel
    printf("%d", omp_get_thread_num());

}
