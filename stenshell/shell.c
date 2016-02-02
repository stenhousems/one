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

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// --------------------------------------------
// Implementation for each prototype below
// --------------------------------------------

void parse(char* line, command_t* p_cmd)
{
  int position = 0;
  char **args = malloc(BUFFER * sizeof(char*));
  char *arg;
  // Allocate space for arguments

  arg = strtok(line, " ");
  while (arg != NULL) {
    args[position] = arg;
    position++;

  p_cmd->name = args[0];
  p_cmd->argc = sizeof(args);
  p_cmd->argv = args;
  // Point values to our command_t struct
}


int execute(command_t* p_cmd)
{
  int status = FALSE;
  char* fullpath;

  status = find_fullpath(fullpath, p_cmd);

  if (status) {
    if (fork() == 0) {
      execv(fullpath, p_cmd->argv);
      perror("Execute terminated with an error condition!\n");
      exit(TRUE);
    }
    wait(&status);
  } else {
    perror("Command not found!\n");
  }

  return TRUE; // Thanks to Tony Leclerc for example code
}


int find_fullpath(char* fullpath, command_t* p_cmd)
{
  struct stat buffer;
  int exists;
  // string that represents the fully qualified
  // path of a file or directory on the file system
  char* file_or_dir;

  exists = stat( file_or_dir, &buffer );

  if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) )
    return TRUE;
  else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) )
    return TRUE;
  else
    return FALSE;
}

int is_builtin(command_t* p_cmd)
{
  int i = 0;
  do {
    if(p_cmd->name != valid_builtin_commands[i])
      i++;
  } while(i < sizeof(valid_builtin_commands));
  // Compare given command with built in commands

  if(i == sizeof(valid_builtin_commands))
    return FALSE;
  else
    return TRUE;
  // FALSE if index went through entire array
}

int do_builtin(command_t* p_cmd)
{
  if(p_cmd->argv[0] == "cd")
    return chdir(argv[1]);
  else
    return ERROR;
  // Obviously only works for cd command
}

void cleanup(command_t* p_cmd)
{
  free(p_cmd->name);
  free(p_cmd->argv);

  *p_cmd->name = NULL;
  *p_cmd->argv = NULL;
  // Free space and set pointers to NULL
}
