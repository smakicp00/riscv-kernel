#include "../h/Semaphore.hpp"
#include "../h/PeriodicThread.hpp"
Semaphore* *Semaphore::sems[5];


List<TCB> Semaphore::suspendedQueue;
void Semaphore::signal() {
    if (++val > 0)unblock();
}

void Semaphore::wait() {
    int i;
    __asm__ volatile("mv %0, a1":"=r"(i));
    if(i!=1){
        if (val == 0)block();
        else {
            val--;
            //if(val == 0)block();
        }
    }
    else{
        if (val > 0) val--;
    }
}

void Semaphore::unblock() {
    if(suspendedQueue.peekLast()!= nullptr){TCB *tcb = suspendedQueue.removeLast();
        Scheduler::put(tcb);}
}

void Semaphore::block() {
    TCB *old = TCB::running;
    suspendedQueue.addFirst(old);
    TCB::running = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::running->context);
}

int Semaphore::sem_destroy(){
    while(suspendedQueue.peekLast()!= nullptr){
        TCB *tcb = suspendedQueue.removeLast();
        Scheduler::put(tcb);
    }
    return 1;
}
void Semaphore::check(){
    List<TCB>::Elem* cur = suspendedQueue.getHeadNode();

    while (cur) {
        List<TCB>::Elem* next = suspendedQueue.getNextNode(cur);
        TCB* thread = suspendedQueue.getNodeData(cur);
        if (thread->isPeriodic()){

            PeriodicThread *thread_tmp = (PeriodicThread*)thread;
            int i = thread_tmp->getPeriod();
            i--;
            thread_tmp->setPeriod(i);

            if (i == 0) {
                Scheduler::put(thread);
                suspendedQueue.deleteNode(cur);
            }
        }
        cur = next;
    }
}