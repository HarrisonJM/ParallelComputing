#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAXRAND 10

void setupMatrix(int** matrix, int maxX, int maxY);
void multiplyForRow(int* m1,
					int m1x,
					int* m2,
					int m2y,
					int** resultMatrix,
					int commonNumber);
int getNumber(int* matrix, int column, int row, int maxColumns);
void printMatrix(const char*, int* matrix, int maxX, int maxY);


int main(void)
{
	/* seed the RNG */
	srand(time(NULL));

	int ax=0;
	int ay=0;
	int bx=0;
	int by=0;

	int* matrixA;
	int* matrixB;
	int* resultantMatrix;

	/* Retrieve Matrix Sizes */
	printf("Enter Size of first array (x, y):");
	scanf("%d%d", &ax, &ay);

	printf("Enter Size of second array (x, y):");
	scanf("%d%d", &bx, &by);

	/* Array cannot be multiplied */
	if(ay != bx)
		exit(1);

	matrixA = malloc(ax*ay*(sizeof(int*)));
	matrixB = malloc(bx*by*(sizeof(int*)));
	resultantMatrix = malloc(ax*by*(sizeof(int*)));
	
	setupMatrix(&matrixA, ax, ay);
	setupMatrix(&matrixB, bx, by);

	printMatrix("A", matrixA, ax, ay);

	printMatrix("B", matrixB, bx, by);

	multiplyForRow(matrixA, ax, matrixB, by, &resultantMatrix, ay);
	printMatrix("Result", resultantMatrix, ax, by);
}

void printMatrix(const char* matrID, int* matrix, int maxX, int maxY)
{
	printf("%s\n", matrID);

	for(int x=0; x < maxX; ++x)
	{
		for(int y=0; y < maxY; ++y)
			printf("%d ", matrix[x*maxY+y]);

		printf("\n");
	}
}

int getRandomNumber()
{
    return (rand()%MAXRAND);
}

void setupMatrix(int** matrix, int maxX, int maxY)
{
	for(int x=0; x<maxX; ++x)
		for(int y=0; y<maxY; ++y)
			(*matrix)[x*maxY+y] = getRandomNumber();
}

/*
 *	m1 first matrix
 *	m2 second matrix
 *	result resultantMAtrix
 *	commonNumber the number common to both matrices
 */
void multiplyForRow(int* m1,
					int m1x,
					int* m2,
					int m2y,
					int** resultMatrix,
					int commonNumber)
{
	int curSum=0;
	
	/* index to a row */
	/* match that to a column */
	/* indexed by commonNumber (70) */
	/* */

	for(int x=0; x < m1x; ++x)
		for(int y=0; y < m2y; ++y)
		{
			for(int cn=0; cn < commonNumber; ++cn)
			{
				curSum+= getNumber(m1, cn, x, commonNumber) * getNumber(m2, y, cn, m2y);
                printf("%d * %d \n", getNumber(m1, cn, x, commonNumber), getNumber(m2, y, cn, m2y));
			}

			printf("curSum: %d\n", curSum);
			(*resultMatrix)[(x*m2y)+y] = curSum;
            curSum=0;
		}		

		printf("\n");
}

int getNumber(int* matrix, int column, int row, int maxColumns)
{
	return matrix[(row*maxColumns)+column];
} 





















