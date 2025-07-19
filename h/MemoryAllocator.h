#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include "../h/print.hpp"
#include "../h/list.hpp"


struct Node{
    char* addr;
    Node* next;
    Node* prev;
    bool free;
    size_t size;
};



class MemoryAllocator {

private:

    static Node* head;

public:

    static void init();
    static void *mem_new(size_t n);
    static bool mem_free(void* ptr);
};

#endif //MEMORYALLOCATOR_H
