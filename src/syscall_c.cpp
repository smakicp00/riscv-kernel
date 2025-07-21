#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.h"
#include "../h/Semaphore.hpp"
#include "../h/PeriodicThread.hpp"

void call(uint64 op){
    __asm__ volatile("mv a0, %0"::"r"(op):);
    __asm__ volatile("ecall");
}

uint64 ret(){
    uint64 ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}

void* mem_alloc(size_t size) {
    __asm__ volatile("mv a1, %0"::"r"(size):);
    call(0x01);
    return (void*)ret();
}
void mem_free(void* ptr){
    __asm__ volatile("mv a1, %0"::"r"(ptr):);
    call(0x02);
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
	__asm__ volatile("mv a3, %0"::"r"(arg):);
	__asm__ volatile("mv a2, %0"::"r"(start_routine):);
    __asm__ volatile("mv a1, %0"::"r"(handle):);
    call(0x11);
    return 1;
}

int thread_exit(){
	call(0x12);
    int r = ret();
    return r;
}

void thread_dispatch(){
    call(0x13);
}

int sem_open(sem_t* handle, unsigned init){
    __asm__ volatile("mv a2, %0"::"r"(init):);
    __asm__ volatile("mv a1, %0"::"r"(handle):);
    call(0x21);
    return 1;
}

int sem_close(sem_t handle){
    //if(!handle) return 0;
    //handle->sem_destroy();
    return 1;
}

int sem_wait(sem_t id){
    __asm__ volatile("mv a1, %0"::"r"(id):);
    call(0x23);
	return 1;
}

int sem_signal(sem_t id){
    __asm__ volatile("mv a1, %0"::"r"(id):);
    call(0x24);
	return 1;
}

int sem_timedwait(sem_t id, time_t timeout){
    __asm__ volatile("mv a2, %0"::"r"(timeout):);
    __asm__ volatile("mv a1, %0"::"r"(id):);
    call(0x25);
    return 1;
}

int sem_trywait(sem_t id){
    __asm__ volatile("mv a1, %0"::"r"(id):);
    call(0x26);
    return 1;
}


int thread_p_create(thread_t *handle, void (*start_routine)(void *), void *arg, time_t period){
    *handle = PeriodicThread::createPeriodicThread(period,start_routine, arg);
    return 1;
}

int time_sleep(time_t t){
    __asm__ volatile("mv a1, %0"::"r"(t):);
    call(0x31);
    return 1;
}

void putc(char c){
    __asm__ volatile("mv a1, %0"::"r"(c):);
    call(0x41);
}

int getc(){
    call(0x42);
    return 1;
}

void print_string(char const *string){
    __asm__ volatile("mv a1, %0"::"r"(string):);
    call(0x51);
}
void print_int(uint64 string){
    __asm__ volatile("mv a1, %0"::"r"(string):);
    call(0x52);
}