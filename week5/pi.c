#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include "timer.h"

#define NUMTHREADS 100

/* Serial Code */
static long const num_steps = 100000;

double piSerial()
{
    double step;

    int i;
    double x, pi, sum = 0.0;

    step = 1.0/(double)num_steps;
    for(i=0; i<num_steps; i++)
    {
        x = (i+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }

    pi = step * sum;

    return pi;
}

double piASync()
{
    double step = 1.0/(double)num_steps;

    double sumFinal = 0.0;

    int i;
#pragma omp parallel for private(i) reduction(+:sumFinal)
    for(i = 0; i < num_steps; ++i)
    {
        double x = (i+0.5)*step;
        sumFinal += 4.0/(1.0+x*x);
    }

    return (step * sumFinal);
}

double piAsync2()
{
    int i, nthreads;
    double pi, sum[NUMTHREADS];
    double step = 1.0/(double)num_steps;
    omp_set_num_threads(NUMTHREADS);

    #pragma omp parallel
    {
       int j, id, nthrds;
       double x;
       id = omp_get_thread_num();
       nthrds = omp_get_num_threads();

       if (id == 0)
           nthreads = nthrds;

       for(j = id, sum[id] = 0.0; j < num_steps; j+=nthreads)
       {
           x = (j+0.5) * step;
           sum[id] += 4.0/(1.0+x*x);
       }
    }

    for(i = 0, pi = 0.0; i < nthreads; ++i)
        pi += sum[i] * step;

    return pi;
}

int main()
{
    startCount();
    printf("%f\n", piSerial());
    stopCountAndPrintWithName("Serial    test: ");

    printf("\n");

    startCount();
    printf("%f\n", piASync());
    stopCountAndPrintWithName("Parallel  Test: ");

    printf("\n");

    startCount();
    printf("%f\n", piAsync2());
    stopCountAndPrintWithName("Parallel2 Test: ");
}

