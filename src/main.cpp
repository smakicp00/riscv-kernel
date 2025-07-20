#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.hpp"
#include "../h/Semaphore.hpp"

int main()
{
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    thread_t threads[5];
    thread_p_create(&threads[0], nullptr,nullptr,0);
    TCB::running = threads[0];
    thread_p_create(&threads[1], workerBodyA,nullptr,0);
    printString("ThreadA created\n");
    thread_p_create(&threads[2], workerBodyB,nullptr,0);
    printString("ThreadB created\n");
    thread_p_create(&threads[3], workerBodyC,nullptr,0);
    printString("ThreadC created\n");
    thread_p_create(&threads[4], workerBodyD,nullptr,0);
    printString("ThreadD created\n");

    sem_t sem1;
    sem_open(&sem1,1);
	Semaphore::sems[0] = &sem1;
    //sem_close(*Semaphore::sems[0]);
	Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;
}
