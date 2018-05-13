#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>

void kill(int pid);

int runModule(char * filename, int argc, char * argv[], int background);

int getpid();

int runProcess(void * entryPoint, int argc, char * argv[], int background);

#endif
