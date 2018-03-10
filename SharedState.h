
#ifndef THREADS_LOTTERY_SHAREDSTATE_H
#define THREADS_LOTTERY_SHAREDSTATE_H

typedef struct {
    double accuResult;
    double percentage;
    int iteration;
} ThreadEntry;

extern ThreadEntry* SharedState;

#endif
