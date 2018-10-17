#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <zconf.h>
#include "timer.h"

typedef struct histoChart
{
    int _min;
    int _max;
    int _count;
} histoChart;

void initChart(histoChart** histogram)
{
    for(int i = 0; i < 10; ++i)
    {
        (*histogram)[i]._min = i * 10;
        if(i != 9)
            (*histogram)[i]._min = ((i+1) * 10) - 1;
        else
            (*histogram)[i]._min = 100;

        (*histogram)[i]._count = 0;
    }
}

void numberCounter(int sizeOfList, const int* numberList, histoChart** histogram)
{
	for(int i = 0; i < sizeOfList; ++i)
	{
	    int cnRef = numberList[i];
	    int indexToUpdate = -1;

        for(int j = 0; j < 10; ++j)
        {
            if(0 == j)
                if(cnRef < 10)
                {
                    indexToUpdate = 0;
                    break;
                }

            if(( cnRef >= j*10 ) && (cnRef < (j+1)*10))
                indexToUpdate = 0;
        }

        (*histogram)[indexToUpdate]._count++;
	}
}

void readnumbersIn(int** numberList, int sizeOfList, const char* fileName)
{
	FILE* fp = fopen(fileName, "r");

	char* number;
	char* endptr;

	number = malloc(3);
	int counter = 0;

	while(fgets(number, 3, fp) != NULL)
	{
		(*numberList)[counter] = (int)strtol(number, &endptr, 2);
		++counter;
	}
}

int getNumberOfLines(const char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    int lines = 0;
    while(!feof(fp))
    {
        int ch = fgetc(fp);
        if(ch == '\n')
        {
            lines++;
        }
    }

    fclose(fp);

    return lines;
}

void printNumbersList(int *numbersList,
                      int size)
{
    for(int i = 0; i < size; ++i)
        printf("%d, ", numbersList[i]);
}

int main()
{
    const int numberOfLines = getNumberOfLines("/home/hmarcks/src/ParallelComputing/week4/grades.txt");
    int *numberList = malloc(numberOfLines*sizeof(int));
    readnumbersIn(&numberList,
                  numberOfLines,
        "/home/hmarcks/src/ParallelComputing/week4/grades.txt");

    histoChart* histogram;
    histogram = malloc(10*sizeof(histoChart));
    initChart(&histogram);

    printNumbersList(numberList,
                     numberOfLines);

    startCount();
    for(int i = 0; i < (numberOfLines/5); ++i)
    {
        numberCounter(5,
                      numberList + (i * 5),
                      &histogram);
    }
    stopCountAndPrintWithName("Normal doing (4x25) ");

    startCount();
#pragma omp parallel num_threads(4)
    {
        int numThread = omp_get_thread_num();

        numberCounter(5,
                      numberList + (numThread * 5),
                      &histogram);
    }
    stopCountAndPrintWithName("Threaded doing (4x25) ");



//    numberCounter(25, numberList + 25, &histogram);
//    numberCounter(25, numberList + 50, &histogram);
//    numberCounter(25, numberList + 75, &histogram);
}