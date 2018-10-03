#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int bubbleSort(int** numbersToSort, int maxNumbers);
int generateNumbers(int** dest, int len);
int* intdup(int const * src, int len);
void printNumberArrays(const int* src, const int len);
int quickSort(int** numbersToSort, const int maxNumbers);

int main(int argc, char **argv)
{
	int* numbersToSort;
	int* backupNumbers;

	int maxNumbers = atoi(argv[argc-1]);
	srand(time(NULL));

	/* admin */
	numbersToSort=malloc(maxNumbers*sizeof(int));
	generateNumbers(&numbersToSort, maxNumbers);
	printf("Initial Array\n");
	printNumberArrays(numbersToSort, maxNumbers);
	backupNumbers=intdup(numbersToSort, maxNumbers);

	/* bubble sort */
	printf("Buuble Sort\n");
	clock_t begin = clock();
	bubbleSort(&numbersToSort, maxNumbers);
	clock_t end = clock();
	printNumberArrays(numbersToSort, maxNumbers);
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Bub Sort Time: %f\n", time_spent);

	/* quick sort */
	begin = clock();
	quickSort(&backupNumbers, maxNumbers);	
	end = clock();
	printNumberArrays(backupNumbers, maxNumbers);
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("QuickSort Time: %f\n", time_spent);

	return 0;
}

int bubbleSort(int** numbersToSort, const int maxNumbers)
{
	int counter=0;
	int* ntsRef = *numbersToSort;

	do
	{
		counter = 0;
		for(int i = 0; i < maxNumbers; ++i)
		{
			if(ntsRef[i] < ntsRef[i+1])
			{
				/* swap numbers */
				++counter;
				int temp = ntsRef[i+1];
				ntsRef[i+1] = ntsRef[i];
				ntsRef[i] = temp;
			}
		}

	} while(counter != 0);

	return 0;
}

int shiftNumbersLeft(int** numbersToSort, const int maxNumbers, const int shiftDex)
{
	int* ntsRef = *numbersToSort;
	int shiftNumb = ntsRef[shiftDex];

	for(int i = shiftDex; i < maxNumbers; ++i)
	{
		if(maxNumbers-1 == i-1)
			ntsRef[i] = shiftNumb;
		else
			ntsRef[i] = ntsRef[i+1];
	}

	puts("1");
	return 0;
}

int quickSort(int** numbersToSort, const int maxNumbers)
{	
	static int depth = 0;
	depth++;

	if(maxNumbers <= 1)
		return 0;

	int* ntsRef = *numbersToSort;

	int pivot = ntsRef[maxNumbers-1], pivotIndex=maxNumbers-1;
	for(int i = 0; i < maxNumbers; ++i)
	{
		if(pivot < ntsRef[i])
		{
			shiftNumbersLeft(numbersToSort, maxNumbers, i);
			pivotIndex--;
		}
	}

	int* leftArray, *rightArray;
	int leftArraySize, rightArraySize;
	leftArray = *numbersToSort;
	rightArray = *numbersToSort+pivotIndex;
	leftArraySize = pivotIndex-1;
	rightArraySize = maxNumbers-pivotIndex;
	
	quickSort(&leftArray, leftArraySize);
	quickSort(&rightArray, rightArraySize);
	
}

int generateNumbers(int** dest, const int len)
{
	for(int i = 0; i < len; ++i)
	{
		int randy = rand() %10;
		(*dest)[i] = rand() % 10;
	}

	return 0;
}

int* intdup(int const * src, const int len)
{
   int * p = malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
}

void printNumberArrays(const int* src, const int len)
{
	for(int i = 0; i < len; ++i)
	{
		printf("%d ", src[i]);
	}

	printf("\n");
}
