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

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// --------------------------------------------
// Implementation for each prototype below
// --------------------------------------------
void parse( char* line, command_t* p_cmd ) {
	int length;

	char *buffer = malloc(sizeof(char) * buffsize);
}

int execute( command_t* p_cmd ) {
	return 0;
}

int find_fullpath( char* fullpath, command_t* p_cmd ) {
	return 0;
}

int is_builtin( command_t* p_cmd ) {
	int i;
	for (i = 0; valid_builtin_commands[i] != NULL; i++) {
		if (strcmp(p_cmd->path, valid_builtin_commands[i]))
			return TRUE;	
	}
}

int do_builtin( command_t* p_cmd ) {
	if(strcmp(p_cmd->path, valid_builtin_commands[0])) // input cd
		return chdir(p_cmd->argv[1]);
	else if(strcmp(p_cmd->path, valid_builtin_commands[1])) // input exit
		return 0;

	return 0;
}

void cleanup( command_t* p_cmd ) {
	free(p_cmd->path);
	p_cmd->path = NULL; // free path name

	int i;
	for (i = 0; i < p_cmd->argc; i++) {
		free(p_cmd->argv[i]);
		p_cmd->argv[i] = NULL; // free arg values
	}
	free(p_cmd->argv);
	p_cmd->argv = NULL;

	p_cmd->argc = 0; // free arg counts
}
