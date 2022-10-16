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
#define MAX_BUF 200

int main() {
    //char path[MAX_BUF];
    char cmd[MAX_COMMAND_LENGTH + 1];
    char *params[MAX_NUMBER_OF_PARAMS + 1];
    char hostname[HOST_NAME_MAX + 1];
    int lines = 0;
    int showLines = 0;
    int ok;

    //clear();

    while (ok) {
        char *username = getenv("USER");
        gethostname(hostname, HOST_NAME_MAX + 1);
	//getcwd(path, MAX_BUF);

        lines++;

        if (showLines)
            printf("%d\t%s@%s$ ", lines, username, hostname);
        else
            printf("%s@%s$ ", username, hostname);
        
        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        if (cmd[strlen(cmd) - 1] == '\n') {
            cmd[strlen(cmd) - 1] = '\0';
        }

        parseCmd(cmd, params);

        if (strcmp(params[0], "exit") == 0)
            exit(0);
        
	if (strcmp(params[0], "cd") == 0) {
	    chdir(params[1]);
	    //strcpy(path, params[1]);
	    continue;
	}

        if (strcmp(params[0], "xsh") == 0) {
            if (strcmp(params[1], "zero") == 0) {
                lines = 0;
                continue;
            }
        
            if (strcmp(params[1], "change") == 0) {
                lines = 0;
                showLines = showLines ? 0 : 1;
                continue;
            }
        }

    
        ok = (executeCmd(params) == -1) ? 0 : 1;
  }

  return 0;
}

void parseCmd(char *cmd, char **params) {
    for (int i = 0; i < MAX_NUMBER_OF_PARAMS; i++) {
        params[i] = strsep(&cmd, " ");
        if (params[i] == NULL)
            break;
    }
}

int executeCmd(char **params) {
    int code = execvp(params[0], params);

    if (code == -1)
        printf("%s: error", params[0]);

    return code;
}
