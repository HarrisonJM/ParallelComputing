#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <omp.h>
#include "timer.h"

#define MAXRAND 10

typedef struct matrix
{
    int _x;
    int _y;
    int *_grid;
} matrix_t;

int getNumber(int *matrix,
              int column,
              int row,
              int maxColumns)
{
    return matrix[(row * maxColumns) + column];
}

void printMatrix(const char *matrID,
                 matrix_t *matrix)
{
    printf("%s\n",
           matrID);

    for (int x = 0; x < matrix->_x; ++x)
    {
        for (int y = 0; y < matrix->_y; ++y)
            printf("%5d ",
                   matrix->_grid[x * matrix->_y + y]);

        printf("\n");
    }
}

int getRandomNumber()
{
    return (rand() % MAXRAND);
}

void setupMatrix(matrix_t *matrix,
                 const int maxX,
                 const int maxY)
{
    matrix->_x = maxX;
    matrix->_y = maxY;
    matrix->_grid = calloc(maxX * maxY,
                           sizeof(int));

    int *grid_ref = matrix->_grid;
    for (int x = 0; x < maxX; ++x)
        for (int y = 0; y < maxY; ++y)
            grid_ref[x * maxY + y] = getRandomNumber();
}

/*
 *	m1 first matrix
 *	m2 second matrix
 *	result resultantMAtrix
 *	commonNumber the number common to both matrices
 */
void multiplyForRow(matrix_t *m1,
                    matrix_t *m2,
                    matrix_t *resultMatrix,
                    int commonNumber)
{
    int curSum = 0;

    /* index to a row */
    /* match that to a column */
    /* indexed by commonNumber (70) */
    /* */

    for (int x = 0; x < m1->_x; ++x)
        for (int y = 0; y < m2->_y; ++y)
        {
            for (int cn = 0; cn < commonNumber; ++cn)
            {
                curSum += getNumber(m1->_grid,
                                    cn,
                                    x,
                                    commonNumber)
                    * getNumber(m2->_grid,
                                y,
                                cn,
                                m2->_y);
            }

            resultMatrix->_grid[(x * (m2->_y)) + y] = curSum;
            curSum = 0;
        }

    printf("\n");
}

void asyncMult1(const matrix_t *m1,
                const matrix_t *m2,
                matrix_t *resultMatrix,
                const int commonNumber)
{

    /* index to a row */
    /* match that to a column */
    /* indexed by commonNumber (70) */
    /* */
    int x, y, cn;
    int *mat1GridRef = m1->_grid,
        *mat2GridRef = m2->_grid;
    int maxM1X = m1->_x;
    int maxM2Y = m1->_y;

    for (x = 0; x < maxM1X; ++x)
        for (y = 0; y < maxM2Y; ++y)
        {
            int curSum = 0;
#pragma omp parallel for reduction(+:curSum)
            for (cn = 0; cn < commonNumber; ++cn)
            {
                curSum += getNumber(mat1GridRef,
                                    cn,
                                    x,
                                    commonNumber)
                    * getNumber(mat2GridRef,
                                y,
                                cn,
                                maxM2Y);
            }

            resultMatrix->_grid[(x * (maxM2Y)) + y] = curSum;
        }

    printf("\n");
}

/*
 * Calculates a singel point
 */
int calcPoint(const matrix_t *m1,
              const matrix_t *m2,
              int row,
              int column,
              int commonNumber)
{
    int curSum = 0;
    for (int cn = 0; cn < commonNumber; ++cn)
    {
        curSum += m1->_grid[(row * commonNumber) + cn]
            * m2->_grid[(cn * commonNumber) + column];
    }

    return curSum;
}

void crawler(const matrix_t *m1,
             const matrix_t *m2,
             matrix_t *resultMatrix,
             const int curXPos,
             const int curYPos,
             const int crawlCut,
             const int commonNumber)
{
    // Reached the top
    if (curXPos < 0)
        return;

    if (curYPos > m2->_x)
        return;

    // Calculate our single point
    resultMatrix->_grid[(0 * resultMatrix->_y) + resultMatrix->_y] =
        calcPoint(m1,
                  m2,
                  curYPos,
                  curXPos,
                  commonNumber);

#pragma omp parallel
    crawler(m1,
            m2,
            resultMatrix,
            curXPos - 1,
            curYPos,
            1,
            commonNumber);

    if (crawlCut != 0)
        #pragma omp parallel
        crawler(m1,
                m2,
                resultMatrix,
                curXPos,
                curYPos + 1,
                0,
                commonNumber);
}

/*
 * fetch next adjacent diagonals
 */
void asyncMult2(const matrix_t *m1,
                const matrix_t *m2,
                matrix_t *resultMatrix,
                const int commonNumber)
{
    crawler(m1,
            m2,
            resultMatrix,
            resultMatrix->_x,
            0,
            0,
            commonNumber);
}

int main(void)
{
    /* seed the RNG */
    srand(time(NULL));

    int ax = 10;
    int ay = 10;
    int bx = 10;
    int by = 10;

//    /* Retrieve Matrix Sizes */
//    printf("Enter Size of first array (x, y):");
//    scanf("%d%d", &ax, &ay);
//
//    printf("Enter Size of second array (x, y):");
//    scanf("%d%d", &bx, &by);

    /* Array cannot be multiplied */
    if (ay != bx)
        exit(1);

    matrix_t matrixA, matrixB, resultantMatrix;

    setupMatrix(&matrixA,
                ax,
                ay);
    setupMatrix(&matrixB,
                bx,
                by);
    setupMatrix(&resultantMatrix,
                ax,
                by);

    printMatrix("A",
                &matrixA);
    printMatrix("B",
                &matrixB);

    startCount();
    multiplyForRow(&matrixA,
                   &matrixB,
                   &resultantMatrix,
                   ay);
    stopCountAndPrintWithName("First Try: ");
    printMatrix("Result",
                &resultantMatrix);

    printf("\n");

    matrix_t resMat2;
    setupMatrix(&resMat2,
                ax,
                by);
    startCount();
    asyncMult1(&matrixA,
               &matrixB,
               &resMat2,
               ay);
    stopCountAndPrintWithName("2nd   Try: ");
    printMatrix("Result",
                &resMat2);

    printf("\n");

    matrix_t resMat3;
    setupMatrix(&resMat3,
                ax,
                by);
    startCount();
    asyncMult2(&matrixA,
               &matrixB,
               &resMat3,
               ay);
    stopCountAndPrintWithName("3rd   Try: ");
    printMatrix("Result",
                &resMat3);
}
