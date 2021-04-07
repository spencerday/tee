/*
 * Author: Spencer Day
 *
 * Implementation of the tee command (read from stdin and write to stdout and 
 * files), with the append option
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"

#define ERROR_MSG "Usage: tee [-a]... [file]..."

void usageAndExit()
{
    fprintf(stderr, ERROR_MSG);
    exit(EXIT_FAILURE);
}

/*
 * first pass over arguments to check for append flag and get the number of files
 */
FILE** argCheck(int argc, char *argv[], int* append, int *numFiles)
{
    int i;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strlen(argv[i]) != 2 || argv[i][1] != 'a') {
                fprintf(stderr, "tee: invalid option '%s'\n", argv[i]);
                usageAndExit();
            }
            else {
                *append = 1;
            }
        }
        else {
            (*numFiles)++;
        }
    }
    (*numFiles)++;
    return malloc(sizeof(FILE*) * (*numFiles));
}


int main(int argc, char *argv[])
{
    int numFiles = 0, append = 0;
    FILE** files = argCheck(argc, argv, &append, &numFiles);


    /* make sure malloc worked, and then open files for writing */
    if (NULL == files) {
        perror("realloc error");
        exit(EXIT_FAILURE);
    }
    files[0] = stdout;  /* first output is always stdout */
    openFiles(argv, argc, files, append);

    /* main loop */
    tee(files, numFiles);

    /* cleanup */
    closeFiles(files, numFiles);
    free(files);

    exit(EXIT_SUCCESS);
}
