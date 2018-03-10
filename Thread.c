#include <signal.h>
#include "Thread.h"

Thread* Thread_New(long threadID, void *function, long tickets, long work, double yieldPercentage){
    Thread* this = (Thread*) (malloc(sizeof(Thread)));
    address_t sp, pc;

    this->threadID = threadID;
    this->completed = 0;
    this->tickets = tickets;
    this->work = work;
    this->yieldPercentage = yieldPercentage;

    sp = (address_t) this->stack + STACK_SIZE - sizeof(address_t);
    pc = (address_t) function;

    sigsetjmp(this->context,1);
    (this->context->__jmpbuf)[JB_SP] = translate_address(sp);
    (this->context->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&this->context->__saved_mask); // initializes saves mask signal to empty

    return this;
}

#ifdef __x86_64__ // code for 64 bit Intel arch
//A translation required when using an address of a variable
//Use this as a black box in your code.
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
            "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

#else
//A translation required when using an address of a variable
//Use this as a black box in your code.
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
		"rol    $0x9,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}
#endif