#include <stdlib.h>

void multiplyForRow(int*[50][70],
					int*[70][80],
					int*[50[80],
					int commonNumber);
int getNumber(int**, int column, int row);
int setupMatrix(int**, int x, int y);

int main(void)
{
	int matrixA[50][70];
	int matrixB[70][80];
	int resultantMatrix[50][80] = {0};
	
	setupMatrix(&matrixA, 50, 70);
	setupMatrix(&matrixB, 70, 80);
	/* result modulo = y */

}

int getRandomNumber()
{
	srand(time(NULL));
	
    return (rand() % 1000);
}

int setupMatrix(int*[][80] matrix, int maxX, int maxY)
{
	for(int x=0; x<maxX; ++x)
		for(int y=0; y<maxY; ++y)
			*((*matrix)+x)+y = getRandomNumber();
}

/*
 *	m1 first matrix
 *	m2 second matrix
 *	result resultantMAtrix
 *	commonNumber the number common to both matrices
 */
void multiplyForRow(int[][70] m1, 
				    int[][80] m2, 
					int*[][80] result,
					const int commonNumber)
{
	int result=0;
	
	/* index to a row */
	/* match that to a column */
	/* indexed by commonNumber (70) */
	/* */

	for(int x=0; x < 50; ++x)
		for(int y=0; y < 80; ++y)
		{
			for(int cn=0; cn < commonNumber; ++cn)
				result+= getNumber(m1, cn, x) * getNumber(m2, y, cn);

			(*((*result)+c)+y) = result;
		}		
}

int getNumber(int[][] matrix, int column, int row)
{
	int** m1 = matrix;

	return (m1+=column)+=row;
} 





















