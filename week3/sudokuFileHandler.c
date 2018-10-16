#include <string.h>

#include "sudokuFileHandler.h"
#include "fileHandling.h"
#include "sudokuUtility.h"

FILE* openSudoku(const char* sudokuName)
{
    FILE* fp = NULL;
    int fileOpened = checkExists(sudokuName);

    // Open file for writing ASudoku
    if(!fileOpened)
    {
        fp = openFile(sudokuName, "rw");
        //createSudoku(fp);
    }
    else //Open file just for reading the ASudoku
    {
        fp = openFile(sudokuName, "r");
    }

    return fp;
}

void fillArrayWithValuesFromFile(FILE *fp,
                                 int **sudokuGrid)
{
    // numbers + commas + newline
    size_t lineLength =
        (SUDOKUSIZE + (SUDOKUSIZE-2) + 1) * sizeof(char);

    char *line = NULL;
    line = malloc(lineLength+1);
    int *sud_ref = *sudokuGrid;

    char *svptr = NULL;

    for(int numberOfLines = 0; numberOfLines < SUDOKUSIZE; ++numberOfLines)
    {
        getline(&line, &lineLength, fp);

        char *anumber = strtok_r(line, ",", &svptr);
        int i = 0;

        while(anumber != NULL)
        {
            int number = atoi(anumber);

            sud_ref[(numberOfLines*SUDOKUSIZE) + i] = number;

            anumber = strtok_r(NULL, ",", &svptr);
            ++i;
        }

        svptr = NULL;
    }

    free(line);
}
