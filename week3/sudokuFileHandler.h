

#ifndef PROTOCOLDEVELOPER_SUDOKUFILEHANDLER_H
#define PROTOCOLDEVELOPER_SUDOKUFILEHANDLER_H


#include <stdlib.h>
#include <stdio.h>


FILE* openSudoku(const char* sudokuName);
void fillArrayWithValuesFromFile(FILE *fp,
                                 int **sudokuGrid);

#endif //PROTOCOLDEVELOPER_SUDOKUFILEHANDLER_H
