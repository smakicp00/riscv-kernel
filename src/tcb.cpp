#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg)
{
    return new TCB(body, TIME_SLICE, arg);
}

void TCB::yield()
{
    uint64 op = 0x13;
    __asm__ volatile("mv a0, %0"::"r"(op):);
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}

int TCB::threadKill(){
    if(running->body == nullptr){
        return -1;
    }
    else{
        running->setFinished(true);
        return 1;
    }
}