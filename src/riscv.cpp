#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/print.hpp"
#include "../h/MemoryAllocator.h"
#include "../lib/mem.h"
uint64 r_instruction(){
    uint64 ins;
    __asm__ volatile("mv %0, a0":"=r"(ins));
    return ins;
}

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    ms_sstatus(SSTATUS_SPP);
    uint64 scause = r_scause();
    uint64 ins = r_instruction();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)

        if(ins == 0x01){
            uint64 volatile sepc = r_sepc()+4;
			 uint64 volatile sstatus = r_sstatus();
            size_t n;
            __asm__ volatile("mv %0, a1":"=r"(n));
            void* ptr = MemoryAllocator::mem_new(n);
			//void* ptr = __mem_alloc(n);
            __asm__ volatile("mv a0, %0\n" "sd a0, 10*8(fp)" :: "r"(ptr): "a0", "memory");
            w_sepc(sepc);
			w_sstatus(sstatus);
        }
        else if(ins == 0x02){
            uint64 volatile sepc = r_sepc()+4;
			uint64 volatile sstatus = r_sstatus();
            void* ptr ;
            __asm__ volatile("mv %0, a1":"=r"(ptr));
            //__mem_free(ptr);
            MemoryAllocator::mem_free(ptr);
            w_sepc(sepc);
			w_sstatus(sstatus);
        }
        else{
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause

    }
}