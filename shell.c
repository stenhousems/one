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
char *read(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

void parse(char* line, command_t* p_cmd)
{
   while (*source) {
      *target = *source;
      source++;
      target++;
   }
   *target = '\0';

}


int execute(command_t* p_cmd)
{
  int status;
  pid_t pid = fork();

  if (pid == 0) {
// This is the child
// Execute in same environment as parent
    execv(args[0], args);
    perror("Execute terminated with an error condition!\n");
    exit(1);
  }
// This is the parent - wait for the child to terminate
  wait(&status);

  return 1;
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
    return 0;
  else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) )
    return 0;
  else
    return -1;
}

int is_builtin(command_t* p_cmd)
{
  int i = 0;
  do {
    if(p_cmd.name != valid_builtin_commands[i])
      i++;
  } while(i < sizeof(valid_builtin_commands));
  // Compare given command with built in commands

  if(i == sizeof(valid_builtin_commands))
    return 0;
  else
    return 1;
  // FALSE if index went through entire array
}

int do_builtin(command_t* p_cmd)
{
  struct stat buffer;
  int exists;
  // string that represents the fully qualified
  // path of a file or directory on the file system
  char* file_or_dir;

  exists = stat( file_or_dir, &buffer );

  if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) )
    return 0;
  else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) )
    return 0;
  else
    return -1;
}

void cleanup(command_t* p_cmd)
{
  free(p_cmd.name);
  free(p_cmd.argv);

  p_cmd.name -> NULL;
  p_cmd.argv -> NULL;
  // Free space and set pointers to NULL
}
