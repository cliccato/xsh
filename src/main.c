#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void parseCmd(char *cmd, char **params);
int executeCmd(char **params);

#define clear() printf("\e[1;1H\e[2J")
#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 10
#define HOST_NAME_MAX 20

int main() {
  char cmd[MAX_COMMAND_LENGTH + 1];
  char *params[MAX_NUMBER_OF_PARAMS + 1];
  char hostname[HOST_NAME_MAX + 1];
  int cmdNumber = 0;
  int showRow = 0;

  // clear();

  while (1) {
    // Print command prompt
    char *username = getenv("USER");
    gethostname(hostname, HOST_NAME_MAX + 1);

    cmdNumber++;

    if (showRow)
      printf("%d\t%s@%s$ ", cmdNumber, username, hostname);
    else
      printf("%s@%s$ ", username, hostname);

    // Read command from standard input, exit on Ctrl+D
    if (fgets(cmd, sizeof(cmd), stdin) == NULL)
      break;

    // Remove trailing newline character, if any
    if (cmd[strlen(cmd) - 1] == '\n') {
      cmd[strlen(cmd) - 1] = '\0';
    }

    // Split cmd into array of parameters
    parseCmd(cmd, params);

    // Exit?
    if (strcmp(params[0], "exit") == 0)
      break;

    if (strcmp(params[0], "xsh") == 0) {
      if (strcmp(params[1], "zero") == 0) {
        cmdNumber = 0;
        continue;
      }
  
      if (strcmp(params[1], "change") == 0) {
        cmdNumber = 0;
        showRow = showRow ? 0 : 1;
        continue;
      }
    }

    // Execute command
    if (executeCmd(params) == 0)
      continue;
  }

  return 0;
}

// Split cmd into array of parameters
void parseCmd(char *cmd, char **params) {
  for (int i = 0; i < MAX_NUMBER_OF_PARAMS; i++) {
    params[i] = strsep(&cmd, " ");
    if (params[i] == NULL)
      break;
  }
}

int executeCmd(char **params) {
  // Fork process
  pid_t pid = fork();

  // Error
  if (pid == -1) {
    char *error = strerror(errno);
    printf("fork: %s\n", error);
    return 1;
  }

  // Child process
  else if (pid == 0) {
    // Execute command
    execvp(params[0], params);

    // Error occurred
    char *error = strerror(errno);
    printf("xsh: %s: %s\n", params[0], error);
    return 0;
  }

  // Parent process
  else {
    // Wait for child process to finish
    int childStatus;
    waitpid(pid, &childStatus, 0);
    return 1;
  }
}
