/* Use this for sudoku A
    1,2,3,0,0,0,0,0,0
    0,0,0,1,2,3,0,0,0
    0,0,0,0,0,0,1,2,3
    4,5,6,0,0,0,0,0,0
    0,0,0,4,5,6,0,0,0
    0,0,0,0,0,0,4,5,6
    7,8,9,0,0,0,0,0,0
    0,0,0,7,8,9,0,0,0
    0,0,0,0,0,0,7,8,9

 */

#include <string.h>
#include <time.h>

#include "sudokuFileHandler.h"
#include "sudokuUtility.h"


int solveForRows(int** sudokuGrid)
{
    int solved = 0;

    for(int row = 0; row < SUDOKUSIZE; ++row)
    {
        numbers numberStruct;
        initNumberStruct(&numberStruct);
        for (int column = 0; column < SUDOKUSIZE; ++column)
        {
            // If 0 then fail
            if (!addPresentNumberToStruct((*sudokuGrid)[column*SUDOKUSIZE+row], &numberStruct))
            {
                return 0;
            }
        }
    }

    // All rows solved
    return 1;
}

int solveForSquares(int** sudokuGrid)
{

}

void rowSolver(const int* sudokuGrid)
{
    int numberOfSavedGrids = 1;
    int** previousGrids;
    previousGrids = malloc(numberOfSavedGrids*sizeof(int));

    // Add inital grid to prvious grids
    size_t size = copyPuzzle(previousGrids, sudokuGrid);
    numberOfSavedGrids++;
    // Generate new grid
    int *newGrid = fillPuzzle(sudokuGrid);

    do
    {
        // Don't test the same grid twice
        while (!memcmp(previousGrids, newGrid,size))
        {
            // adding a new grid
            numberOfSavedGrids++;
            // realloc more space
            int **previousGrids_new = realloc(previousGrids,
                                              numberOfSavedGrids * sizeof(int));
            // set new pointer
            previousGrids = previousGrids_new;
            // Add last generated Grid
            previousGrids[numberOfSavedGrids - 1] = newGrid;
            // Generate new grid
            newGrid = fillPuzzle(sudokuGrid);
        }

        printPuzzle(newGrid);

    }while(!solveForRows(&newGrid));

    // Grid solved
}

int main(void)
{
    // Acquire files
    srand(time(NULL));

    FILE* SudokuA_fp = openSudoku("/home/hmarcks/src/ParallelComputing/week3/ASudoku");
    FILE* SudokuB_fp = openSudoku("/home/hmarcks/src/ParallelComputing/week3/BSudoku");
    FILE* SudokuC_fp = openSudoku("/home/hmarcks/src/ParallelComputing/week3/CSudoku");

//    int *sudokuA = NULL;
//    sudokuA = malloc(SUDOKUSIZE * sizeof(int));
//    fillArrayWithValuesFromFile(SudokuA_fp,
//                                &sudokuA);

    const int sudokuA[] =
        {
            1,2,3,0,0,0,0,0,0,
            0,0,0,1,2,3,0,0,0,
            0,0,0,0,0,0,1,2,3,
            4,5,6,0,0,0,0,0,0,
            0,0,0,4,5,6,0,0,0,
            0,0,0,0,0,0,4,5,6,
            7,8,9,0,0,0,0,0,0,
            0,0,0,7,8,9,0,0,0,
            0,0,0,0,0,0,7,8,9
        };
    const int sudokuB[] =
        {
            1,2,3,0,0,0,7,8,9,
            9,0,0,4,5,6,0,0,1,
            0,0,0,1,0,0,0,0,0,
            7,8,9,0,0,0,1,2,3,
            0,1,0,9,8,7,0,4,0,
            0,0,0,0,0,5,0,0,0,
            2,3,1,0,0,0,4,5,6,
            0,4,0,2,3,4,0,0,7,
            0,0,0,0,5,0,0,0,0
        };
    const int sudokuC[] =
        {
            1,2,3,4,5,6,7,8,9,
            4,5,6,7,8,9,1,2,3,
            7,8,9,1,2,3,4,5,6,
            3,1,2,6,4,5,9,7,8,
            6,4,5,9,7,8,3,1,2,
            2,3,1,5,6,4,8,9,7,
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,

        };

//    printPuzzle(sudokuA);
    rowSolver(sudokuA);

    return 0;
}