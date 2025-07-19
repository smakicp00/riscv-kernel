#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.h"

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
    //__asm__ volatile("mv a1, %0"::"r"(r):);
    //void* r = MemoryAllocator::mem_new(size);
    //return r;
}
void mem_free(void* ptr){
    __asm__ volatile("mv a1, %0"::"r"(ptr):);
    call(0x02);
    //MemoryAllocator::mem_free(ptr);
}
