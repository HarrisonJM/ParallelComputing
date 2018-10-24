#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include "timer.h"

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

int main()
{
    startCount();
    printf("%f\n", piSerial());
    stopCountAndPrintWithName("Serial test: ");

    printf("\n");

    startCount();
    printf("%f\n", piASync());
    stopCountAndPrintWithName("Parallel Test: ");
}

