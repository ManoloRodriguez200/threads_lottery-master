#include <glib.h>
#include "FileLoader.h"

const char *SEPARATOR = ":";

const char *SEPARATOR_LIST = ",";

const char *PREEMPTIVE_TAG = "preemptive";
const char *NUM_THREADS_TAG = "numThreads";
const char *TICKETS_TAG = "tickets";
const char *WORK_TAG = "work";
const char *QUANTUM_TAG = "quantum";
const char *PERCENTAGE_TAG = "yieldPercentage";
const char *COMMENT_TAG = "//";

//Free any allocated memory of the File Loader
void FileLoader_Free(FileLoader *this) {
    free(this->tickets);
    free(this->work);
    free(this);
}

int startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

void FileLoader_DebugPrint(){
    printf("DEBUG FILE LOADER\n");
    printf("preemptive: %d\n", Loader->preemptive);
    printf("numThreads: %ld\n", Loader->numThreads);
    printf("quantum: %d\n", Loader->quantum);
    printf("yieldPercentage: %lf\n", Loader->yieldPercentage);
    int i;
    printf("work: ");
    for(i = 0; i < Loader->numThreads; i++){
        printf("%ld, ", Loader->work[i]);
    }
    printf("\ntickets: ");
    for(i = 0; i < Loader->numThreads; i++){
        printf("%ld, ", Loader->tickets[i]);
    }
    printf("\n");
}

//Creates a new File Loader
void FileLoader_Init(char *fileName) {
    Loader = (FileLoader*) (malloc(sizeof(FileLoader)));
    FILE *settingsFile = fopen(fileName, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (settingsFile == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, settingsFile)) != -1) {
        char *token;
        token = strtok(line, SEPARATOR);
        if(startsWith(COMMENT_TAG, token)){
            break;
        }
        else if (strcmp(token, PREEMPTIVE_TAG) == 0) {
            token = strtok(NULL, SEPARATOR);
            Loader->preemptive = (int) strtol(token, (char **)NULL, 10);
        } else if (strcmp(token, NUM_THREADS_TAG) == 0) {
            token = strtok(NULL, SEPARATOR);
            Loader->numThreads = (int) strtol(token, (char **)NULL, 10);
            Loader->tickets = (long*) malloc(Loader->numThreads * sizeof(long));
            Loader->work = (long*) malloc(Loader->numThreads * sizeof(long));
        } else if (strcmp(token, TICKETS_TAG) == 0) {
            token = strtok(NULL, SEPARATOR);
            char *work = strtok(token, SEPARATOR_LIST);
            int index = 0;
            while(work != NULL && index < Loader->numThreads){
                Loader->tickets[index] = strtol(work, (char **)NULL, 10);
                work = strtok(NULL, SEPARATOR_LIST);
                index++;
            }
        } else if (strcmp(token, WORK_TAG) == 0) {
            token = strtok(NULL, SEPARATOR);
            char *work = strtok(token, SEPARATOR_LIST);
            int index = 0;
            while(work != NULL && index < Loader->numThreads) {
                Loader->work[index] = strtol(work, (char **) NULL, 10);
                work = strtok(NULL, SEPARATOR_LIST);
                index++;
            }
        } else if (strcmp(token, QUANTUM_TAG) == 0) {
            token = strtok(NULL, SEPARATOR);
            Loader->quantum = (unsigned int) strtol(token, (char **) NULL, 10);
        } else if(strcmp(token, PERCENTAGE_TAG) == 0){
            token = strtok(NULL, SEPARATOR);
            Loader->yieldPercentage = g_strtod (token, NULL);
        } else {
            printf("Unknown setting: %s\n", token);
            exit(EXIT_FAILURE);
        }
    }

    fclose(settingsFile);
    if (line)
        free(line);
}

