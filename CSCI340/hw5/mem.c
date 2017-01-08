#include <stdlib.h>   // for malloc() and free()
#include <stdio.h>

#include "mem.h"

/*
  Physical memory array. This is a static global array for all functions in this file.  
  An element in the array with a value of zero represents a free unit of memory.
*/
static unsigned int* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is a static global 
 variable used by functions in this file.  

 */
static unsigned int mem_size;

/*
 The last_placement_position variable contains the end position of the last 
 allocated unit used by the next fit placement algorithm.  
 */
static unsigned int last_placement_position;

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the units of this 
  block must be set to the value, duration.
 */

int mem_allocate(mem_strategy_t strategy, unsigned int size, unsigned int duration) {

	int count = 0, start_pos = 0, blocksize = 0;
	int i = 0, done = 0, startOver = 0;
	
	int best = 0, best_pos = 0;

	// First fit
 	if (strategy == FIRST) {

		for (i = 0; i < mem_size; i++) {
			if (memory[i] == 0) {
				// Find first empty block
				start_pos = i;
			}
			
			else {
				// No free block
				return -1;
			}
			
			while (i < mem_size && !done) {
				for (i = start_pos; i < mem_size; i++) {
					// Get chunk size
					if (memory[i] == 0) {
						blocksize++;
					}
					else {
						break;
					}
				}
				
				if (blocksize <= 0) { // Continue to check for larger chunk
					i++;
				}
				
				else if (blocksize >= size) {
					memory[start_pos] = duration;
					done = 1;
				}
	      
				else {
					count++;
					i += blocksize;
				}
			}
			
			if (done) {
				return count;
			}
			
			else {
				return -1;
			}
		} 
	}

	// Next fit
	else if (strategy == NEXT) {
		// Begin at last_placement_position
		i = last_placement_position;
		
		while (i < mem_size && !done) {
			for (i = start_pos; i < mem_size; i++) {
				// Get chunk size
				if (memory[i] == 0) {
					blocksize++;
				}
				
				else {
					break;
				}
			}
				
			if (blocksize <= 0) {
				i++;
			}

			else if (blocksize >= size) {
				memory[start_pos] = duration;
				last_placement_position = i;
				done = 1;
			}
	      
			else {
				count++;
				i += blocksize;
					
				if (i >= mem_size && !startOver) {
					startOver = 1;
					i = i % mem_size;
				}
			}
		}
			
		if (done) {
			return count;
		}
			
		else {
			return -1;
		}
	}

	// Best fit
	else if(strategy == BEST) {
		
		for (i = 0; i < mem_size; i++) {
			if (memory[i] == 0) {
				// Find first empty block
				start_pos = i;
			}
			else {
				// No free block
				return -1;
			}
			
			while (i < mem_size && !done) {
				for (i = start_pos; i < mem_size; i++) {
					// Get chunk size
					if (memory[i] == 0) {
						blocksize++;
					}
					else {
						break;
					}
				}
			
				if (blocksize <= 0) {
					i++;
				}

				else if (blocksize > size && blocksize < best) {
					best_pos = i;
					best = blocksize;
					i += blocksize;
					done = 1;
				} // Compared bestblock against new blocks
				else {
					count++;
					i += blocksize;
				}
			}

			if (done) {
				memory[best_pos] = duration;
				return count;
			}
			else
				return -1;
		}
	}

	return blocksize;

}

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
 */
int mem_single_time_unit_transpired() {
	
	int i = 0;
	
	while (i < mem_size) {
		if (memory[i] > 0) {
			memory[i]--;
		}

		i++;
	}

	return 0;
}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size) {
	
	int fragCount = 0, length = 0;
	int i;
	
	for (i = 0; i < mem_size; i++) {
		if (memory[i] == 0) {
			length++;
		}

		else {
			if (length <= frag_size && length > 0) {
				fragCount++;
			}

			length = 0;
		}
	}

	return fragCount;

}

/*
  Set the value of zero to all entries of memory.
 */
void mem_clear() {
	
	int i = 0;

	while (i < mem_size) {
		memory[i] = 0;
		i++;
	}
}

/*
 Allocate physical memory to size. This function should 
 only be called once near the beginning of your main function.
 */
void mem_init( unsigned int size ) {

	memory = malloc( sizeof(unsigned int)*size );
	mem_size = size;
}

/*
 Deallocate physical memory. This function should 
 only be called once near the end of your main function.
 */
void mem_free() {
	
	free( memory );
}
