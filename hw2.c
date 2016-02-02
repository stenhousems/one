//  @author Michael Stenhouse
//  @date 2/1/16

// ----------------------------------------------
// These are the only libraries that can be
// used. Under no circumstances can additional
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// -----------------------------------
// Main function
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main(int argc, char** argv) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while (!done)
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
        //      if (parsed command is "exit")
        //      	done = true
	//  	else if (is _builtin(...))
	//			do_builtin(...)
	//		else if (find_fullpath(...))
	//  		execute(...)
	//		else
	//			error()
	// 		cleanup(...)
	//
	int done = 1;

	do {
		printf("> ");
		char cmd = *read();


		parse(cmd, );
		if (argv[0] = "exit")
			done = 0;
		else if (is_builtin(cmd) == 1)
			do_builtin(cmd);
		else if (find_fullpath(cmd) == 1)
			execute(cmd);
		else
			perror("ERROR!\n");
		cleanup(cmd);
	} while(done = 1);

	printf("Thank you for using StenhouseShell.");
	return 0;

}
