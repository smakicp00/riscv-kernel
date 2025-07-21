#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.hpp"

void workerBodyA(void*)
{
    //time_sleep(150);
    //sem_wait(*Semaphore::sems[0]);
    for (uint64 i = 0; i < 10; i++)
    {
        print_string("A: i=");
        print_int(i);
        print_string("\n");
        //thread_exit();
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
    //sem_signal(*Semaphore::sems[0]);
}

void workerBodyB(void*)
{
    //sem_wait(*Semaphore::sems[0]);
    for (uint64 i = 0; i < 16; i++)
    {
        print_string("B: i=");
        print_int(i);
        print_string("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
    //sem_signal(*Semaphore::sems[0]);
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { TCB::yield(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void*)
{
    //sem_timedwait(*Semaphore::sems[0],150);
    uint8 i = 0;
    for (; i < 3; i++)
    {
        print_string("C: i=");
        print_int(i);
        print_string("\n");
    }

    print_string("C: yield\n");
    getc();
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    print_string("C: t1=");
    print_int(t1);
    print_string("\n");

    uint64 result = fibonacci(12);
    print_string("C: fibonaci=");
    print_int(result);
    print_string("\n");

    for (; i < 6; i++)
    {
        print_string("C: i=");
        print_int(i);
        print_string("\n");
    }
    //sem_signal(*Semaphore::sems[0]);
//    TCB::yield();
}

void workerBodyD(void*)
{
    //sem_wait(*Semaphore::sems[0]);
    uint8 i = 10;
    for (; i < 13; i++)
    {
        print_string("D: i=");
        print_int(i);
        print_string("\n");
    }

    print_string("D: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();

    uint64 result = fibonacci(16);
    print_string("D: fibonaci=");
    print_int(result);
    print_string("\n");

    for (; i < 16; i++)
    {
        print_string("D: i=");
        print_int(i);
        printString("\n");
    }
    //sem_signal(*Semaphore::sems[0]);
//    TCB::yield();
}