#ifndef IO_H
#define IO_H

#include <stdio.h>  /* needed for closeFiles declaration */

void openFiles(char* argv[], int argc, FILE** files, int append);
void closeFiles(FILE** files, int numFiles);
void tee(FILE** files, int numFiles);

#endif
