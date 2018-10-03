#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int bubbleSort(int** numbersToSort, int maxNumbers);
int generateNumbers(int** dest, int len);
int* intdup(int const * src, int len);
void printNumberArrays(const int* src, const int len);
int quickSort(int** numbersToSort, const int lo, const int hi);

int main(int argc, char **argv)
{
	int* numbersToSort;
	int* backupNumbers;

//    int maxNumbers = atoi(argv[argc-1]);
    int maxNumbers = 9;
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
    printf("Quick Sort\n");
	begin = clock();
    quickSort(&backupNumbers, 0, maxNumbers-1);
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

void swapNumbers(int* src, int* dest)
{
    int temp = *src;
    *src = *dest;
    *src = temp;
}

int getPivotIndex(int maxNumbers)
{
    // If odd, use the middle number
    int pivotIndex = 0;
    if (maxNumbers%2)
    {
        pivotIndex = (maxNumbers/2)+1;
    }
    else
    {
        pivotIndex = maxNumbers/2;
    }

    return pivotIndex;
}

int partition(int** numbersToSort, const int lo, int hi)
{

    int* ntsRef = *numbersToSort;
    int pivot = ntsRef[getPivotIndex(lo)];

    int i = lo - 1;
    int j = hi + 1;

    while(true)
    {
        do
            ++i;
        while(ntsRef[i] < pivot);

        do
            j--;
        while(ntsRef[j] > pivot);

        if( i >= j)
            return j;

        int temp = ntsRef[i];
        ntsRef[i] = ntsRef[j];
        ntsRef[j] = temp;
    }
}

int quickSort(int** numbersToSort, const int lo, const int hi)
{
    if (lo < hi)
    {
        int partitionPoint = partition(numbersToSort,
                                       lo,
                                       hi);
        quickSort(numbersToSort, lo, partitionPoint);
        quickSort(numbersToSort, partitionPoint + 1, hi);
    }
}

int generateNumbers(int** dest, const int len)
{
	for(int i = 0; i < len; ++i)
	{
		int randy = rand() %10;
		(*dest)[i] = randy;
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
