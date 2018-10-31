#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <zconf.h>
#include <time.h>
#include "timer.h"

const int numberSListLocal[] =
    {35, 83, 32, 66, 68, 38, 86, 13, 96, 11, 10, 70, 4, 11, 22, 60, 55, 3, 10,
     54, 50, 75, 47, 75, 54, 46, 89, 52, 30, 32, 58, 57, 32, 31, 59, 1, 55, 100,
     92, 12, 59, 89, 60, 25, 80, 2, 76, 45, 74, 76, 36, 59, 82, 2, 86, 69, 3,
     39, 2, 11, 84, 90, 58, 7, 18, 38, 81, 62, 98, 21, 73, 99, 51, 99, 63, 46,
     94, 3, 59, 75, 95, 90, 16, 76, 41, 17, 50, 97, 51, 19, 39, 80, 78, 32, 41,
     21, 7, 88, 50, 33};

typedef struct histoChart_struct
{
    int _min;
    int _max;
    int _count;
} histoChart;

void initChart(histoChart **histogram)
{
    for (int i = 0; i < 10; ++i)
    {
        (*histogram)[i]._min = i * 10;

        if (i != 9)
            (*histogram)[i]._max = ((i + 1) * 10) - 1;
        else
            (*histogram)[i]._max = 100;

        (*histogram)[i]._count = 0;
    }
}

void numberCounter(int sizeOfList,
                   const int *numberList,
                   histoChart **histogram)
{
    for (int i = 0; i < sizeOfList; ++i)
    {
        int cnRef = numberList[i];
        int indexToUpdate = -1;

        for (int j = 0; j < 10; ++j)
        {
            if (0 == j)
                if (cnRef < 10)
                {
                    indexToUpdate = 0;
                    break;
                }

            if ((cnRef >= j * 10) && (cnRef < (j + 1) * 10))
            {
                indexToUpdate = j;
                break;
            }
        }

        (*histogram)[indexToUpdate]._count++;
    }
}

void numberCounterThreaded(int sizeOfList,
                           int index,
                           const int *numberList,
                           histoChart ***histogram)
{
    numberCounter(sizeOfList,
                  numberList,
                  (*histogram) + index);
}

void readnumbersIn(int **numberList,
                   int sizeOfList,
                   const char *fileName)
{
    FILE *fp = fopen(fileName,
                     "r");

    char *number;
    char *endptr;

    number = malloc(3);
    int counter = 0;

    while ((fgets(number,
                  3,
                  fp) != NULL) && (counter <= sizeOfList))
    {
//	    long numL = (int)strtol(number, &endptr, 2);
        (*numberList)[counter] = (int) strtol(number,
                                              &endptr,
                                              2);
        ++counter;
    }
}

int getNumberOfLines(const char *fileName)
{
    FILE *fp = fopen(fileName,
                     "r");

    int lines = 0;
    while (!feof(fp))
    {
        int ch = fgetc(fp);
        if (ch == '\n')
        {
            lines++;
        }
    }

//    fclose(fp);

    return lines;
}

void printNumbersList(int *numbersList,
                      int size)
{
    for (int i = 0; i < size; ++i)
        printf("%d, ",
               numbersList[i]);
}

void printHistogram(const histoChart *histogram)
{
    int totCount = 0;

    for (int i = 0; i < 10; ++i)
    {
        printf("%3d -> %3d : ",
               histogram[i]._min,
               histogram[i]._max);
        for (int j = 0; j < histogram[i]._count; ++j)
        {
            printf("*");
            totCount++;
        }

        printf("\n");
    }

    printf("Total values: %d\n", totCount);
}

void addHistograms(histoChart **histograms,
                   histoChart **histoOut,
                   int numCharts)
{
    histoChart *histOutRef = *histoOut;

    for (int i = 0; i < numCharts; ++i)
    {
        const histoChart *allHistoRef = histograms[i];

        for (int j = 0; j < 10; ++j)
            histOutRef[j]._count += allHistoRef[j]._count;
    }
}

int main()
{
    const int numberOfLines = 1
        + getNumberOfLines("/home/hmarcks/src/ParallelComputing/week4/grades.txt");

    char *str = malloc(20);
    printf("%s",
           str);

    int *numberList = calloc(1400,
                             4);
    readnumbersIn(&numberList,
                  numberOfLines,
                  "/home/hmarcks/src/ParallelComputing/week4/grades.txt");

    histoChart *histogram;
    histogram = calloc(10,
                       12);
    initChart(&histogram);

    startCount();
    numberCounter(100,
                  numberSListLocal,
                  &histogram);

    stopCountAndPrintWithName("Normal doing (4x25)");

    printHistogram(histogram);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    histoChart **histogram2;
    // Four pointers to histogram arrays
    histogram2 = calloc(4,
                        4);
    for (int i = 0; i < 4; ++i)
    {
        histogram2[i] = calloc(10,
                               12);
        initChart((histogram2 + i));
    }

    startCount();
#pragma omp parallel num_threads(4)
    {
        int numThread = omp_get_thread_num();

        numberCounterThreaded(25,
                              numThread,
                              numberSListLocal,
                              &histogram2);

//        printf("threadNum: %d\n", numThread);
    }
    stopCountAndPrintWithName("Others doing (4x25)");

    histoChart *histogramOut;
    histogramOut = calloc(10,
                          12);
    initChart(&histogramOut);

    addHistograms(histogram2,
                  &histogramOut,
                  4);
    printHistogram(histogramOut);
}