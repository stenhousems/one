#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  number of times to repeat each experiment (ie. number of runs).  The
  third parameter is the duration of the each simulation run. The
  forth parameter is a random number seed. Here is an example command
  line:

  ./hw7 10000 100 2000 1234

  This means that your program should initialize physical memory to
  10,000 units, perform 100 runs with each run taking 2000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1234.
*/

int main(int argc, char** argv)
{
	// Verify number of parameters
	if (argc != 5) {
		printf("Invalid input. Read the instructions!\n");
		return 1;
	}

	// Turn parameters into int values
	int memSize = (int) atoi(argv[1]);
	int runs = (int) atoi(argv[2]);
	int time = (int) atoi(argv[3]);
	int seed = (int) atoi(argv[4]);

	// Many counters for each strategy
	int firstFrags = 0, firstProbes = 0;
	int firstTotalFrags = 0, firstTotalProbes = 0, firstTotalFails = 0;
	int firstAvgFrags = 0, firstAvgProbes = 0, firstAvgFails = 0;

	int nextFrags = 0, nextProbes = 0;
	int nextTotalFrags = 0, nextTotalProbes = 0, nextTotalFails = 0;
	int nextAvgFrags = 0, nextAvgProbes = 0, nextAvgFails = 0;

	int bestFrags = 0, bestProbes = 0;
	int bestTotalFrags = 0, bestTotalProbes = 0, bestTotalFails = 0;
	int bestAvgFrags = 0, bestAvgProbes = 0, bestAvgFails = 0;

	int i, j;
	int duration, size;
	
	// Init memory and random seed
	mem_init(memSize);
	srand(seed);
	
	for (int method = 0; method < 3; method++) {

		mem_clear();		

		for (i = 0; i < runs; i++) {

			//mem_clear();

			for (j = 0; j < time; j++) {

				duration = rand() % (MAX_DURATION - MIN_DURATION);
				size = rand() % (MAX_REQUEST_SIZE - MIN_REQUEST_SIZE);
				
				switch (method) {
					case 0: // First fit
						firstProbes = mem_allocate(FIRST, size, duration);
						if (firstProbes == -1)
							firstTotalFails++;
						else
							firstTotalProbes += firstProbes;
						
						mem_single_time_unit_transpired();
						firstFrags = mem_fragment_count(MAX_REQUEST_SIZE);
						firstTotalFrags += firstFrags;
						
						break;

					case 1: // Next fit
						nextProbes = mem_allocate(NEXT, size, duration);
						if (nextProbes == -1)
							nextTotalFails++;
						else
							nextTotalProbes += nextProbes;
						
						mem_single_time_unit_transpired();
						nextFrags = mem_fragment_count(MAX_REQUEST_SIZE);
						nextTotalFrags += nextFrags;

						break;

					case 2: // Best fit
						bestProbes = mem_allocate(BEST, size, duration);
						if (bestProbes == -1)
							bestTotalFails++;
						else
							bestTotalProbes += bestProbes;

						mem_single_time_unit_transpired();
						bestFrags = mem_fragment_count(MAX_REQUEST_SIZE);
						bestTotalFrags += bestFrags;

						break;
				}
			}
		}
	}

	// Output
	printf("FIRST FIT RESULTS: \n");
	firstAvgFrags = firstTotalFrags / (double) runs;
	firstAvgProbes = firstTotalProbes / (double) runs;
	firstAvgFails = firstTotalFails / (double) runs;
	printf("\tAverage fragment count: %d \n", firstAvgFrags);
	printf("\tAverage probes: %d \n", firstAvgProbes);
	printf("\tAverage failures: %d \n\n", firstAvgFails);

	printf("NEXT FIT RESULTS:\n");
	nextAvgFrags = nextTotalFrags / (double) runs;
	nextAvgProbes = nextTotalProbes / (double) runs;
	nextAvgFails = nextTotalFails / (double) runs;
	printf("\tAverage fragment count: %d \n", nextAvgFrags);
	printf("\tAverage probes: %d \n", nextAvgProbes);
	printf("\tAverage failures: %d \n\n", nextAvgFails);

	printf("BEST FIT RESULTS:\n");
	bestAvgFrags = bestTotalFrags / (double) runs;
	bestAvgProbes = bestTotalProbes / (double) runs;
	bestAvgFails = bestTotalFails / (double) runs;
	printf("\tAverage fragment count: %d \n", bestAvgFrags);
	printf("\tAverage probes: %d \n", bestAvgProbes);
	printf("\tAverage failures: %d \n\n", bestAvgFails);

	mem_free();

	return 0;
}
