#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include "colors.h"
#include "command.h"
#include "args.h"

#define DEBUG
#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 10
#define HOST_NAME_MAX 20
#define PATH_MAX 200
#define COMMAND_HISTORY "~/.xsh_history"
#define clear() printf("\e[1;1H\e[2J")

void printShell(char path[], char hostname[], char username[]) {
    printf(RED"%s@%s$ "RESET, username, hostname);
}

void welcome() {
    printf("Welcome to xsh version %s\n", getVersion());
    printf("Type help for instructions\n\n");
}

void main(int argc, char *argv[]) {
    if (argc > 1)
        execArgs(argc, argv);

    char path[PATH_MAX];
    char cmd[MAX_COMMAND_LENGTH + 1];
    char *params[MAX_NUMBER_OF_PARAMS + 1];
    char hostname[HOST_NAME_MAX + 1];
    int line = 0;

    clear();
    welcome();

    while(1) {
        char *username = getenv("USER");
        gethostname(hostname, HOST_NAME_MAX + 1);
	getcwd(path, PATH_MAX);

        line++;
	printShell(path, hostname, username);
        
        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        if (cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = '\0';

        parseCmd(cmd, params);

        if (strcmp(params[0], "exit") == 0)
            exit(1);
        
	else if (strcmp(params[0], "cd") == 0) {
	    chdir(params[1]);
	    strcpy(path, params[1]);
	    continue;
	}

	else if (strcmp(params[0], "help") == 0) {
	    helpMessage();continue;
	}
	
	else if (strcmp(params[0], "version") == 0) {
	    version();continue;
	}

        else if (strcmp(params[0], "line") == 0) {
	    printf("%d\n", line);continue;
	}

	if (!executeCmd(params))
	    break;
    
    }

}
