
#ifndef THREADS_LOTTERY_TIMER_H
#define THREADS_LOTTERY_TIMER_H

extern long selected_quantum;

void setup_scheduler_timer(unsigned int quantum);

void set_next_alarm();
int catch_signal(int sig,void(*handler)(int));
void invoke_scheduler(int sig);

#endif
