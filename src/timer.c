#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "timer.h"

static clock_t begin;
static clock_t end;
//clock_t end = clock();

void startCount()
{
    begin = clock();
}

void stopCountAndPrintWithName(const char* testing)
{
    end = clock();
    clock_t timeSpent = end - begin;

    printf("%s Time: %Lf\n", testing, (long double)timeSpent/CLOCKS_PER_SEC);
    end = (long)0;
    begin = (long)0;
}

void stopCountAndPrint()
{
    stopCountAndPrintWithName("");
}