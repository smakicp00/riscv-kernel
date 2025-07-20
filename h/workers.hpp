#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_WORKERS_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_WORKERS_HPP

#include "../h/Semaphore.hpp"
#include "../h/syscall_c.hpp"

extern void workerBodyA(void*);

extern void workerBodyB(void*);

extern void workerBodyC(void*);

extern void workerBodyD(void*);

#endif
