#include "../h/Console.hpp"
#include "../lib/hw.h"

void Console::putc(char c) {
    volatile uint64 *uart2 =  (volatile uint64*)CONSOLE_RX_DATA;
    while((CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT) != 0){}
    *uart2 = (uint64)c;
}

void getcBody(void*){

    uint64 op = 0x10;
    __asm__ volatile("mv a1, %0"::"r"(op):);
    while(1){}

}