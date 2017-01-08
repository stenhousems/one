/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *
 ********************************************************
 * mtx.c
 *
 * Simple multi-threaded example.
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

static pthread_mutex_t mlock;

void th1_count( int* x );
void th2_count( int* x );
void delay( long nanosec );

int main(int argc, char** argv ) {

	int x = 0;

	pthread_mutex_init( &mlock, NULL );

  	pthread_t thread1, thread2;
 
  	if (pthread_create(&thread1, 
		 NULL,
		 (void *) th1_count,
		 (void *) &x ) != 0)
		perror("pthread_create"), exit(1); 

  	if (pthread_create(&thread2, 
		 NULL, 
		 (void *) th2_count,
		 (void *) &x ) != 0)
		perror("pthread_create"), exit(1); 
  
  	if (pthread_join(thread1, NULL) != 0)
		perror("pthread_join"),exit(1);

  	if (pthread_join(thread2, NULL) != 0)
		perror("pthread_join"),exit(1);


  return 0; 
}

void th1_count(int* x ) {

	int i;
  
	for (i = 0;  i < 30; i++) {

		pthread_mutex_lock( &mlock );
    		printf("thread[1,%d]: x = %d\n", i, (*x) );
		(*x) += 1; 
		pthread_mutex_unlock( &mlock );
		delay( 3000 );
  	}

}

void th2_count(int* x ) {

	int i;
  
	for (i = 0;  i < 30; i++) {
		pthread_mutex_lock( &mlock );
    		printf("thread[2,%d]: x = %d\n", i, (*x) );
		(*x) += 1; 
		pthread_mutex_unlock( &mlock );
		delay( 1000 );
  	}

}

void delay( long nanosec ) {

	struct timespec t_spec;
	
	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );
}

