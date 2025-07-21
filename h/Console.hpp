#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "../h/riscv.hpp"

extern void getcBody(void*);

class Console {
public:
    static void putc(char c);
};



#endif //CONSOLE_HPP
