// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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


void parse( char* line, command_t* p_cmd ) {

	// -----------------------------------------
	// Create some variables

	//int i;

	char* p_line;
	char* p_begin;

	int space_cnt = 0;	// number of counted spaces in entered cmd
	int cmd_length = 0;	// number of entered characters in entered cmd

	char tmp[100];

	// -----------------------------------------
	// Set argc to 0 -> no command was entered by user

	p_cmd->argc = 0;

	// -----------------------------------------
	// Remove multiple white spaces in front, between 
	// arguments, and at the end of the command
	// i.e. trim leading and trailing white space 
	// and only want 1 space between arguments.
	// if your solution doesn't check for this
	// that is ok

	// first trim leading white space (i.e. simply move
	// pointer to the first non-white space character)
	while ( (*line) == ' ' && (*line) != '\0') line++;

	while ( (*line) != '\0' ) {

		if ( (*line) == ' ' ) {

			tmp[cmd_length++] = (*line);

			space_cnt++;
			line+=1;

			while ( (*line) == ' ' ) line+=1;

			// if the last character found after
			// a sequence of white spaces is the 
			// EOL character, then decrement the 
			// space count by 1, this fixes problems
			// of several spaces after command or
			// after the last argument (i.e. trims 
			// trailing white spaces)
			if ( (*line) == '\0' ) space_cnt--;

		} else {

			tmp[cmd_length++] = (*line);
			line+=1;

		}

	}

	tmp[cmd_length]='\0';

	if ( space_cnt == 0 && cmd_length > 0 ) {

		// if ( DEBUG ) printf("[%d,%d] %s\n", space_cnt, cmd_length, tmp );

		p_cmd->argc = space_cnt+1;

		sscanf( tmp, "%s", p_cmd->argv[0] );
		sscanf( tmp, "%s", p_cmd->path );

		if (  ( find_fullpath( tmp, p_cmd ) == FALSE ) && ( is_builtin( p_cmd ) == FALSE ) ) {
			p_cmd->argc = ERROR;
		}

	} else if ( cmd_length > 0 && space_cnt > 0 ) {

		// if ( DEBUG ) printf("[%d,%d] %s\n", space_cnt, cmd_length, tmp );

		p_cmd->argc = space_cnt+1;

		p_line = tmp;
		p_begin = tmp;
		space_cnt = 0;

		while( (*p_line) != '\0' ) {

			if ( (*p_line) == ' ' && space_cnt == 0 ) {

				(*p_line)='\0';

				sscanf( p_begin, "%s", p_cmd->argv[space_cnt] );
				sscanf( p_begin, "%s", p_cmd->path );
				
				if (  ( find_fullpath( p_begin, p_cmd ) == FALSE ) && ( is_builtin( p_cmd ) == FALSE ) ) {
					p_cmd->argc = ERROR;
				}

				p_begin=p_line+1;
				space_cnt++;
				
			} else if ( (*p_line) == ' ' && space_cnt > 0 ) {
				
				(*p_line)='\0';
	            		sscanf( p_begin, "%s", p_cmd->argv[space_cnt] );
	            		p_begin=p_line+1;
	            		space_cnt++;

			}

			p_line++;

		}

		sscanf( p_begin, "%s", p_cmd->argv[space_cnt] );

	} 

} // end parse function


int execute( command_t* p_cmd ) {

	int status = SUCCESSFUL;
	int child_process_status;

	pid_t child_pid;

	if ( p_cmd->argc != ERROR  ) {

		child_pid = fork();

		if ( child_pid == 0 ) {
			p_cmd->argv[ p_cmd->argc ] = NULL;
			execv( p_cmd->path, p_cmd->argv );
			perror( "child process terminated in error condition!" );
			exit(1);

		} else if ( child_pid < 0 ) {

			perror( "Unable to fork child process!" );

		}
		
		if ( wait( &child_process_status ) < 0 )
			status = ERROR;

	} 
	
	return status;

} // end execute function


int find_fullpath( char* command_name, command_t* p_cmd ) {


	char* path = getenv( "PATH" );
	char* p_path = path;

	char fullpath[200];

	int j = 0;
	int k = 0;
	
	struct stat buff;
	
	int exists = FALSE;

	while ( (*p_path) != '\0' ) {
		p_path+=1;
	}

	(*p_path) = ':';
	p_path+=1;
	(*p_path) = '\0';

	while ( (*path) != '\0' ) {

		if ( (*path) != ':' ) { 

			fullpath[j++] = (*path);

		} else {

			fullpath[j++] = '/';

			while ( p_cmd->path[k] != '\0') {

				fullpath[j++] = p_cmd->path[k++];

			}

			fullpath[j] = '\0';

			j=0;
			k=0;

			if ( ( stat( fullpath, &buff ) == 0 && ( S_IFREG & buff.st_mode ) ) ) {

				sscanf( fullpath, "%s", p_cmd->path );
				exists = TRUE;
				break;

			} 

		}

		path+=1;

	}

	return exists;

} // end find_fullpath function


int is_builtin( command_t* p_cmd ) {

	int cnt = 0;

	while ( valid_builtin_commands[cnt] != NULL ) {

		if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

			return TRUE;

		}

		cnt++;

	}

	return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

	// only builtin command is cd

	if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

	struct stat buff;
	int status = ERROR;

	if ( p_cmd->argc == 1 ) {

		// -----------------------
		// cd with no arg
		// -----------------------
		// change working directory to that
		// specified in HOME environmental 
		// variable

		status = chdir( getenv("HOME") );

	} else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


		// -----------------------
		// cd with one arg 
		// -----------------------
		// only perform this operation if the requested
		// folder exists

		status = chdir( p_cmd->argv[1] );

	} 

	return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

	int i=0;
	
	while ( p_cmd->argv[i] != NULL ) {
		free( p_cmd->argv[i] );
		i++;
	}

	free( p_cmd->argv );
	free( p_cmd->path );	

} // end cleanup function


int equals( char* str1, const char* str2 ) {

	// First check length

	int len[] = {0,0};

	char* b_str1 = str1;
	const char* b_str2 = str2;

	while( (*str1) != '\0' ) { 
		len[0]++;
		str1++;
	}

	while( (*str2) != '\0' ) {
		len[1]++;
		str2++;
	}

	if ( len[0] != len[1] ) {

		return FALSE;

	} else {

		while ( (*b_str1) != '\0' ) {

			// don't care about case (you did not have to perform
			// this operation in your solution

			if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

				return FALSE;

			}

			b_str1++;
			b_str2++;

		}

	} 

	return TRUE;


} // end compare function definition
