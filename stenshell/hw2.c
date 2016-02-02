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
int main(int argc, char** argv)
{
	int done = 0;
	char line[BUFFER]; // Array of size 1024

	char* fullpath;
	command_t cmd;

	// A shell does three things:
	// Read, Parse, Execute

	do {
		printf("$ "); // prompt
		fgets(line, BUFFER, stdin); // Read
		parse(line, &cmd); // Parse
		if (argv[0] = "exit")
			done = 1;
		else if (is_builtin(cmd) == 1)
			do_builtin(cmd);
		else if (find_fullpath(fullpath, cmd) == 1)
			execute(cmd); // Execute
		else
			perror("ERROR!\n");
		cleanup(cmd);
	} while(done = 0);

	printf("Thank you for using StenhouseShell.");
	return 0;
}
