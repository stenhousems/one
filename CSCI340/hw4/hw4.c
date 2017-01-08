#include <stdio.h>
#include "dpsim.h"

static const unsigned int NUM_PHILOSOPHERS = 5;

/**************************************************

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread 
	- If the return value != 0, then display an error message and 
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {

	pthread_t main_thread[NUM_PHILOSOPHERS];
	int status = 0;
	int i; // 1 (variables)

	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		if (pthread_create(&main_thread[i], NULL, (void *) th_phil, (void *) i) != 0) {
			printf("Error!");
			return 1;
		}
	} // 2

	delay(1000); //sleep

	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		if (pthread_join(main_thread[i], NULL) != 0) {
			printf("Error!");
			return 2;
		}
	} // 3 (wait for join of philosopher threads)

	return status;

} // end main function
