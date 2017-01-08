#include "dpsim.h"


static const unsigned int NUM_PHILOSOPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosophers[5];

int done = 0;

void* th_main( void* th_main_args ) {
	
	/*	1. Init chop array elements to -1
 		2. Create a thread for each philosopher
 		3. Infinite loop that detects deadlock
 		4. Kill each philosopher thread
 		5. Exit the main thread with status value equal to 0.	*/

	int i;

	for(i = 0; i < NUM_PHILOSOPHERS; i++)
		chopsticks[i]= -1; // 1

	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		if (pthread_create(&philosophers[i], NULL, (void *) th_phil, (void *) i) != 0) {
			printf("Error!");
			exit(1);
		}
	} // 2 (create 5 philosopher threads)

	while(done = 0) { // 3
		if (done = 0) { // deadlock condition (dummy condition... how do I find this?)
			printf("Deadlock condition! Terminating!");
			done = 1;
		}	
	
		else { // no deadlock
			printf("Philospher(s) %d are eating\n", philosophers[i]);
			delay(3000);
		}
	}

	for(i = 0; i < NUM_PHILOSOPHERS; i++) {
		pthread_kill(philosophers[i], NULL);
	} // 4?	

	pthread_exit(NULL); // 5?

} // end th_main function


void* th_phil( void* th_phil_args ) {

	int phil_id = (int) th_phil_args;

	while(TRUE) {
		delay(3000);
		eat(phil_id);
	}
	
} // end th_phil function


void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {
	
	/* 	1. Lock right chop, update chop array, delay
		2. Lock left chop, update chop array, delay
		3. Delay
		4. Unlock left chop, update chop array
		5. Unlock right chop, update chop array	*/

	int right_chopstick = phil_id;
	pthread_mutex_lock(&mutex[right_chopstick]);
	chopsticks[right_chopstick] = phil_id;
	delay(3000); // 1

	int left_chopstick = (phil_id + 1) % NUM_PHILOSOPHERS;
	pthread_mutex_lock(&mutex[left_chopstick]);
	chopsticks[left_chopstick] = phil_id;
	delay(3000); // 2

	delay(9000); // 3

	pthread_mutex_unlock(&mutex[left_chopstick]);
	chopsticks[left_chopstick] = -1; // 4

	pthread_mutex_unlock(&mutex[right_chopstick]);
	chopsticks[right_chopstick] = -1; // 5

} // end eat function
