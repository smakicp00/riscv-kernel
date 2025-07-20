#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP
#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"




class Semaphore {

public:
    static List<TCB> suspendedQueue;
    static Semaphore** sems[5];
    static void check();
public:

    Semaphore(unsigned init = 1):val(init) {
        myHandle = this;
    };
    ~Semaphore();



    void wait();
    void signal();
    int timedwait(time_t t);
    int trywait();
    int sem_destroy();
protected:
    void unblock();
    void block();

private:
    Semaphore* myHandle;
    unsigned val;

};



#endif //SEMAPHORE_HPP
