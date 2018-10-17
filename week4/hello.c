#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello( ) {
  /* get thread id */
  int id = omp_get_thread_num( );
  printf("Hello from thread %d!\n", id);
}

int main(void)
{
	int tid, nthreads; 
	#pragma omp parallel private(tid) 
	{ 
		tid = omp_get_thread_num(); 
		printf("Hello World from thread %d\n", tid); 
		#pragma omp barrier 
		if ( tid == 0 ) 
		{ 
			nthreads = omp_get_num_threads(); 
			printf("Total threads= %d\n",nthreads); 
		} 
	}
 
	#pragma omp parallel num_threads(4)
	hello();


	return 0;
}


