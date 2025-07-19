#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/print.hpp"

void* mem_alloc(size_t size);
void mem_free(void* ptr);


#endif
