#ifndef MAX_NUMBER_OF_PARAMS
#define MAX_NUMBER_OF_PARAMS 10
#endif
#ifndef COMMAND_HISTORY
#define COMMAND_HISTORY "~/.xsh_history"
#endif

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
	if(strcmp(error, "No such file or directory")==0) {
	    printf("Command %s not found\n", params[0]);
	} else {
            printf("%s: %s\n", params[0], error);
	}

        //End Execution
        kill(getpid(), SIGKILL);
        return 0;

    } else {
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}
