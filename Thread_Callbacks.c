#include "Thread_Callbacks.h"
#include "LoteryScheduler.h"
#include "SharedState.h"

void updateSharedState(long threadID, double accuResult, double percentage, int iteration) {
    SharedState[threadID].accuResult = accuResult;
    SharedState[threadID].percentage = percentage;
    SharedState[threadID].iteration = iteration;
}

void updateCallback_preemptive(double accuResult, double percentage, int iteration){
    updateSharedState(Scheduler->currentThread, accuResult, percentage, iteration);
}

void updateCallback_notPreemptive(double accuResult, double percentage, int iteration){
    updateSharedState(Scheduler->currentThread, accuResult, percentage, iteration);
    if(percentage >= Scheduler->threads[Scheduler->currentThread]->yieldPercentage){
        Scheduler->threads[Scheduler->currentThread]->yieldPercentage += Scheduler->yieldPercentage;
        LoteryScheduler_Schedule(Scheduler);
    }
}

void threadFinishedCallback(double accuResult){
    updateSharedState(Scheduler->currentThread, accuResult, 1, -1);
    LoteryScheduler_ThreadCompletes(Scheduler);
}

void runThread() {
    int unitsOfWork = (int) LoteryScheduler_GetWorkOfCurrentThread(Scheduler);
    void (*updateCallback)(double, double, int) = NULL;
    void (*workFinishedCallback)(double) = NULL;
    updateCallback = Scheduler->preemptive ? updateCallback_preemptive : updateCallback_notPreemptive ;
    workFinishedCallback = threadFinishedCallback;
    executeThreadWork(unitsOfWork,updateCallback, workFinishedCallback);
}
