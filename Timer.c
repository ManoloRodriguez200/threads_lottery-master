
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include "Timer.h"
#include "LoteryScheduler.h"

long int selected_quantum = 0;

void setup_scheduler_timer(unsigned int quantum) {
    selected_quantum = quantum;
    catch_signal(SIGALRM, invoke_scheduler);
    //set_next_alarm(selected_quantum);
}

int catch_signal(int sig,void(*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void invoke_scheduler(int sig)
{
    if(Scheduler){
        printf("Thread__: %ld might be preemted\n", Scheduler->currentThread);
        LoteryScheduler_Schedule(Scheduler);
    }
}

void set_next_alarm() {
    struct itimerval new;
    new.it_interval.tv_usec = 0;
    new.it_interval.tv_sec = 0;
    new.it_value.tv_usec = (selected_quantum % 1000) * 1000;
    new.it_value.tv_sec = selected_quantum / 1000;
    setitimer (ITIMER_REAL, &new, NULL);
}
