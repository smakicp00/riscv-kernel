#include "../h/PeriodicThread.hpp"

List<PeriodicThread> PeriodicThread::sleepingThreadsQueue;

time_t PeriodicThread::getPeriod() const {
    return period;
}

void PeriodicThread::setPeriod(time_t period) {
    PeriodicThread::period = period;
}

PeriodicThread* PeriodicThread::createPeriodicThread(time_t period,TCB::Body body, void *arg) {
    return new PeriodicThread(period,body, 2, nullptr);
}
PeriodicThread::~PeriodicThread() {}

void PeriodicThread::check(){
    List<PeriodicThread>::Elem* cur = sleepingThreadsQueue.getHeadNode();

    while (cur) {
    List<PeriodicThread>::Elem* next = sleepingThreadsQueue.getNextNode(cur);
    PeriodicThread* thread = sleepingThreadsQueue.getNodeData(cur);

    int i = thread->getPeriod();
    i--;
    thread->setPeriod(i);

    if (i == 0) {
        Scheduler::put(thread);
        sleepingThreadsQueue.deleteNode(cur);
    }
    cur = next;
    }
}

void PeriodicThread::put(){
    PeriodicThread *old =(PeriodicThread*) TCB::running;
    sleepingThreadsQueue.addFirst(old);
    TCB::running = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::running->context);
}
