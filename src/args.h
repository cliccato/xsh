#ifndef VERSION
#define VERSION "1.1.1"
#endif

void version();
void helpMessage();

void execArgs(int argc, char *argv[]) {
    if (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "--help")==0)
        helpMessage();
    else if (strcmp(argv[1], "-v")==0 || strcmp(argv[1], "--version")==0)
        version();

    exit(0);
}

void version() { printf(VERSION"\n"); }
char* getVersion() { return VERSION; }

void helpMessage() {
    printf("xsh, version "VERSION"\n\n");
    printf("Usage: xsh [option]\n");
    printf("xsh options:\n");
    printf("\t-h | --help\n");     printf("\t-v | --version\n");
}
