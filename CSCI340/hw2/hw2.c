// @author Michael Stenhouse
// @ date 2016/09/26
//
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
#include <string.h>
#include "shell.h"
//
// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//

int main( int argc, char** argv )
{
	command_t cmd;
	int done = FALSE;
	char line[BUFFER];
	char fullpath[BUFFER];

	while (done = FALSE) {
		printf("[stenshell] > "); // prompt
		fgets(line, BUFFER, stdin); // read cmd line
		parse(line, &cmd); // parse cmd line
		
		if(strcmp(cmd.path, "exit")) // "exit" is cmd
			done = TRUE;
		else if (is_builtin(&cmd)) // "cd" is cmd
			do_builtin(&cmd);
		// else if (find_fullpath(fullpath, &cmd)) not working
			// execute(&cmd);
		else
			perror("Invalid input.\n"); // print error
		
		cleanup(&cmd);
	}

	printf("Thank you for using stenshell.");

	return 0;

	// ----------------------------------
	// Original pseudocode for reference
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( find_fullpath( ... ) )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//

} // end main function
