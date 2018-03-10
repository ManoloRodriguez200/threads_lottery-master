#ifndef THREADS_LOTTERY_FILELOADER_H
#define THREADS_LOTTERY_FILELOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int preemptive;
    long numThreads;
    unsigned int quantum;
    double yieldPercentage;
    long* tickets;
    long* work;
}FileLoader;

FileLoader* Loader;



void FileLoader_Free(FileLoader* this);
void FileLoader_DebugPrint();
void FileLoader_Init(char *fileName);

#endif //THREADS_LOTTERY_FILELOADER_H
