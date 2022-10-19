#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 10
#define HOST_NAME_MAX 20
#define PATH_MAX 200
#define clear() printf("\e[1;1H\e[2J")

#define VERSION "1.1.0"

void parseCmd(char *cmd, char **params) {
    for (int i = 0; i < MAX_NUMBER_OF_PARAMS; i++) {
        params[i] = strsep(&cmd, " ");
        if (params[i] == NULL)
            break;
    }
}

int executeCmd(char **params) {
    pid_t pid = fork();

    if (pid == -1) { //ForkCreationError
        char *error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
    }

    if (pid == 0) {
        //Execution
	execvp(params[0], params);

	//Error
        char *error = strerror(errno);
        printf("xsh: %s: %s\n", params[0], error);

	//End Execution
        kill(getpid(), SIGKILL);
        return 0;

    } else {
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}

void main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "--help")==0) {
	    printf("Help message\n");
	    exit(0);

        } else if (strcmp(argv[1], "-v")==0 || strcmp(argv[1], "--version")==0) {
            printf(VERSION);
	    printf("\n");
            exit(0);
        }
    }

    char path[PATH_MAX];
    char cmd[MAX_COMMAND_LENGTH + 1];
    char *params[MAX_NUMBER_OF_PARAMS + 1];
    char hostname[HOST_NAME_MAX + 1];
    int line = 0;

    clear();
    printf("xsh shell\n\n");

    while(1) {
        char *username = getenv("USER");
        gethostname(hostname, HOST_NAME_MAX + 1);
	getcwd(path, PATH_MAX);

        line++;
        printf("%s@%s$ ", username, hostname);
        
        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        if (cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = '\0';

        parseCmd(cmd, params);

        if (strcmp(params[0], "exit") == 0)
            exit(1);
        
	if (strcmp(params[0], "cd") == 0) {
	    chdir(params[1]);
	    strcpy(path, params[1]);
	    continue;
	}

	if (strcmp(params[0], "line") == 0) {
	    printf("%d\n", line);
	    continue;
	}

	if (!executeCmd(params))
	    break;
    
    }

}
