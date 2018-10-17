#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

int numberCounter(int sizeOfList, int max, int* numberList, int** histogram)
{
	for(int i =0; i < sizeoOfList; ++i)
	{


	}

}

void readnumbersIn(int** numberList, int* sizeOfList, const char* filename)
{
	FILE* fp = fopen(filename, "r");

	char* number;
	number = malloc(2);
	int counter = 0;
	
	while((number = getline(&number, 2, fp)) != NULL)
	{
		(*numberList)[counter] = strtol()
	}
}

int main()
{
		

}
