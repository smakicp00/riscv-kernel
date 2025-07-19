#include "../h/MemoryAllocator.h"
static Node head_node;

Node *MemoryAllocator::head = &head_node;

void MemoryAllocator::init() {
    head->addr = (char *) (HEAP_START_ADDR);// + (char*)sizeof(Node*);
    head->next = nullptr;
    head->prev = nullptr;
    head->free = true;
    head->size = (size_t) ((char *) ((char *) HEAP_END_ADDR - (char *) HEAP_START_ADDR) - (char *) sizeof(Node));
}

void *MemoryAllocator::mem_new(size_t n) {

    Node *cur = head;
    while (cur) {
        if (cur->size > n + sizeof(Node) and cur->free) {

            Node *newNode = (Node *) (cur->addr + n + sizeof(Node));//newNode set to start adress of free space
            newNode->addr = cur->addr + n + sizeof(Node);
            newNode->size = cur->size - (n + sizeof(Node));
            newNode->free = true;

            newNode->next = cur->next;
            newNode->prev = cur;

            if (cur->next) { cur->next->prev = newNode; }

            cur->next = newNode;
            cur->free = false;
            cur->size = n + sizeof(Node);

            return (void *) (cur->addr + sizeof(Node));

        } else {
            cur = cur->next;
        }
    }

    return nullptr;
}

bool MemoryAllocator::mem_free(void *ptr) {

    if (ptr == nullptr)return false;
    Node *cur = head;
    while (cur->addr + sizeof(Node) != ptr) {
        cur = cur->next;
    }
    if (cur->next and cur->next->free) {
        cur->size = cur->size + cur->next->size;
        if (cur->next->next)cur->next->next->prev = cur;
        cur->next = cur->next->next;
        cur->free = true;
    }
    if (cur->prev and cur->prev->free) {
        cur->size = cur->size + cur->prev->size;
        if (cur->prev->prev){cur->prev->prev->next = cur;}
        else{head = cur;}
        cur->prev = cur->prev->prev;
        cur->free = true;
    }

    return true;
}