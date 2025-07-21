#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/print.hpp"
#include "../h/tcb.hpp"
#include "../h/Semaphore.hpp"

void* mem_alloc(size_t size);
void mem_free(void* ptr);

class TCB;

typedef TCB* thread_t;
int thread_create (
    thread_t* handle,
    void(*start_routine)(void*),
    void* arg
);

int thread_p_create (
    thread_t* handle,
    void(*start_routine)(void*),
    void* arg,
    time_t period
);

int thread_exit();

void thread_dispatch();

class Semaphore;

typedef Semaphore* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

int sem_timedwait(sem_t id, time_t timeout);

int sem_trywait(sem_t id);

int time_sleep(time_t t);

const int EOF = -1;

int getc();

void putc(char c);

void print_string(char const *string);

void print_int(uint64 string);

#endif
