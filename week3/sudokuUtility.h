#ifndef PROTOCOLDEVELOPER_STRUCTUTILITY_H
#define PROTOCOLDEVELOPER_STRUCTUTILITY_H

#define SUDOKUSIZE 9

typedef struct numbers{
    int* numbersPresent;
    int numberOfPresentNumbers;
} numbers;

void initNumberStruct(numbers* num);
// Returns 0 if number is present in the struct
int addPresentNumberToStruct(int presentNumber, numbers* num);

size_t copyPuzzle(int** dest, const int* src);

void printLine(int* sud, int lineToPrint);
void printPuzzle(const int* sudokuGrid);

int getSemiRandomNumber(numbers* number);
int* fillPuzzle(const int* sudoku);

#endif //PROTOCOLDEVELOPER_STRUCTUTILITY_H
