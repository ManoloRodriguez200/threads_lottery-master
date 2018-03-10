
#include "ThreadWork.h"

#define TERMS_PER_UNIT_OF_WORK 50

void executeThreadWork(int numberOfUnitsOfWork, void (*updateCallback)(double, double, int), void (*workFinishedCallback)(double)) {
    int i = 0;
    double accumulatedResult = 0;
    double lastTerm = 2;

    for (i = 0; i < numberOfUnitsOfWork; i++) {
        lastTerm = executeUnitOfWork(i * TERMS_PER_UNIT_OF_WORK, lastTerm, &accumulatedResult);
        if (i < (numberOfUnitsOfWork - 1))
            updateCallback(accumulatedResult, (double) (i+1) / (double) numberOfUnitsOfWork, i);
    }
    workFinishedCallback(accumulatedResult);
}

double executeUnitOfWork(int initial_n, double lastTerm, double *accumulator) {
    int i;
    double n;
    for (i = initial_n; i < (initial_n + TERMS_PER_UNIT_OF_WORK); i++) {
        n = i;
        *accumulator += lastTerm;
        lastTerm = lastTerm * ((4*n*n + 4*n + 1) / (4*n*n + 10*n + 6));
    }
    return lastTerm;
}
