#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.hpp"
#include "../h/Semaphore.hpp"
#include "../lib/console.h"
int main()
{
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //thread_t threads[5];
    //thread_p_create(&threads[0], nullptr,nullptr,0);
    //getc();
    thread_t threads[5];
    thread_p_create(&threads[0], nullptr,nullptr,0);
    TCB::running = threads[0];
    thread_p_create(&threads[1], workerBodyA,nullptr,0);
    print_string("ThreadA created\n");
    thread_p_create(&threads[2], workerBodyB,nullptr,0);
    print_string("ThreadB created\n");
    thread_p_create(&threads[3], workerBodyC,nullptr,0);
    print_string("ThreadC created\n");
    thread_p_create(&threads[4], workerBodyD,nullptr,0);
    print_string("ThreadD created\n");

    sem_t sem1;
    sem_open(&sem1,1);
	Semaphore::sems[0] = &sem1;

    //sem_close(*Semaphore::sems[0]);
	Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //getc();
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

    print_string("Finished\n");

    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");

    /*uint64 ret;
    char str = __getc();
    putc(str);
    while(1){
        __asm__ volatile("mv %0, a0":"=r"(ret));
        if(ret == 90){
            putc(ret);
        }
    }*/

    //Riscv::w_sie(sie);
    //Riscv::w_sstatus(sstatus);

    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");
    print_string("==================================================================================================\n");




    return 0;
}
