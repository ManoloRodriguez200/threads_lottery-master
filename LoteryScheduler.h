#ifndef THREADS_LOTTERY_LOTERYSCHEDULER_H
#define THREADS_LOTTERY_LOTERYSCHEDULER_H

#include "Thread.h"

typedef struct {
    Thread** threads;
    long numThreads;
    long currentThread;
    int preemptive;
    long completedThreads;
    int playingTickets;
    double yieldPercentage;
    sigjmp_buf context;
    int scheduleComplete;
}LoteryScheduler;

extern LoteryScheduler* Scheduler;


int LoteryScheduler_SaveOwnContext(LoteryScheduler* this); //saves the context of the scheduler
void LoteryScheduler_ResumesOwnContext(LoteryScheduler* this); //resumes the context of the scheduler
void LoteryScheduler_Init(long numThreads, void* function, int preemptive, double yiedlPercentage, long* tickets, long* work);
void LoteryScheduler_Free(LoteryScheduler* this);
int LoteryScheduler_SaveThread(LoteryScheduler* this); //saves current thread context
void LoteryScheduler_ResumeThread(LoteryScheduler* this); //resumes current thread
void LoteryScheduler_Schedule(LoteryScheduler* this);
void LoteryScheduler_ThreadCompletes(LoteryScheduler* this);
void LoteryScheduler_SwitchThreads(LoteryScheduler this);
void LoteryScheduler_SaveResult(LoteryScheduler* this, double result);
long LoteryScheduler_GetWorkOfCurrentThread(LoteryScheduler* this);

#endif //THREADS_LOTTERY_LOTERYSCHEDULER_H
