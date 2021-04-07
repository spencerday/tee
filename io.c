#define _POSIX_SOURCE 1  /* for fileno */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "io.h"


/* 
 * second pass over arguments to open the files in the correct mode
 */
void openFiles(char* argv[], int argc, FILE** files, int append)
{
	char* mode = append ? "a" : "w"; 
	int fileIdx = 1;
	int i;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (NULL == (files[fileIdx++] = fopen(argv[i], mode))) {
				perror("fopen error");
				exit(EXIT_FAILURE);
			}
		}
	}
}

/*
 * close all files except for stdout
 */
void closeFiles(FILE** files, int numFiles)
{
	int i;

	for (i = 1; i < numFiles; i++) {  /* start at 1, we dont want to close stdout */
		if (EOF == fclose(files[i])) {
			perror("fclose error");
			exit(EXIT_FAILURE);
		}
	}
}

/*
 * read from stdin, write to stdout and all other files specified
 */
void tee(FILE** files, int numFiles)
{
	int i;
	char buf[BUFSIZ];
	ssize_t bytesRead = 0;

	/* read until no more input (EOF, or CTRL+C, or CTRL+D) */
	while (0 < (bytesRead = read(STDIN_FILENO, buf, sizeof(buf)))) {
		for (i = 0; i < numFiles; i++) {
			if (-1 == write(fileno(files[i]), buf, bytesRead)) {
				perror("fwrite error");
				exit(EXIT_FAILURE);
			}
		}
	}

	if (-1 == bytesRead) {
		perror("read error");
		exit(EXIT_FAILURE);
	}
}
