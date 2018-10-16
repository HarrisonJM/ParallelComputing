#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sudokuUtility.h"

void initNumberStruct(numbers* num)
{
    num->numberOfPresentNumbers=0;
    num->numbersPresent = malloc(9*sizeof(int));
}

// Returns 0 if number is present in the struct
int addPresentNumberToStruct(int presentNumber, numbers* num)
{
    if(presentNumber != 0)
    {
        for(int i = 0; i < num->numberOfPresentNumbers; ++i)
        {
            if(num->numbersPresent[i] == presentNumber)
                return 0;
        }

        num->numbersPresent[num->numberOfPresentNumbers] = presentNumber;
        num->numberOfPresentNumbers++;
    }
}

size_t copyPuzzle(int** dest, const int* src)
{
    size_t sizeOfPuzzleInBytes = (size_t)81*sizeof(int);
    *dest = malloc(sizeOfPuzzleInBytes);

    // Move basic puzzle into place
    memcpy(dest,
           src,
           sizeOfPuzzleInBytes);

    return sizeOfPuzzleInBytes;
}

void printLine(int* sud, int lineToPrint)
{
    printf("PrintingLine: ");
    for(int i =0; i < SUDOKUSIZE; ++i)
    {
        printf("%d", sud[(lineToPrint*SUDOKUSIZE) + i]);
    }
    printf("\n");
}

void printPuzzle(const int* sudokuGrid)
{
    for(int x = 0; x < SUDOKUSIZE; ++x)
    {
        for (int y = 0; y < SUDOKUSIZE; ++y)
            printf("%d | ",
                   sudokuGrid[(x * SUDOKUSIZE) + y]);

        printf("\n");
    }
}

int getSemiRandomNumber(numbers* number)
{
    int* numbersList = number->numbersPresent;
    int notOkay = 1;
    int randomNumber = (rand() % 9) + 1;

    while(notOkay)
    {
        for(int i = 0; i < number->numberOfPresentNumbers; ++i)
        {
            if(randomNumber != numbersList[i])
            {
                notOkay = 0;
                break;
            }
        }

        randomNumber = (rand() % 9) + 1;
    }

    return randomNumber;
}

int* fillPuzzle(const int* sudoku)
{
    int *filledPuzzle;
    copyPuzzle(&filledPuzzle, sudoku);

    for(int column = 0; column < SUDOKUSIZE; ++column)
    {
        for(int row = 0; row < SUDOKUSIZE; ++row)
        {
            numbers numbersStruct;
            initNumberStruct(&numbersStruct);

            if(filledPuzzle[column*SUDOKUSIZE+row] != 0 )
            {
                addPresentNumberToStruct(filledPuzzle[column*SUDOKUSIZE+row],
                                         &numbersStruct);
            }
            else
            {
                filledPuzzle[column*SUDOKUSIZE+row] = getSemiRandomNumber(&numbersStruct);
            }
        }
    }
}
