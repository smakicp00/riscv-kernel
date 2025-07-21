#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/print.hpp"
#include "../h/MemoryAllocator.h"
#include "../lib/mem.h"
#include "../h/syscall_c.hpp"
#include "../h/PeriodicThread.hpp"
#include "../h/Console.hpp"


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
         else if(ins == 0x11){
            uint64 volatile sepc = r_sepc()+4;
			uint64 volatile sstatus = r_sstatus();

            thread_t* volatile handle;
            using Body = void (*)(void*);
            Body volatile start_routine;
            void* volatile arg;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            __asm__ volatile("mv %0, a2":"=r"(start_routine));
            __asm__ volatile("mv %0, a3":"=r"(arg));

            *handle = TCB::createThread(*start_routine,arg);
            __asm__ volatile("mv a0, %0\n" "sd a0, 10*8(fp)" :: "r"(handle): "a0", "memory");
            w_sepc(sepc);
			w_sstatus(sstatus);
        }
		else if(ins == 0x12){
            uint64 volatile sepc = r_sepc()+4;
			uint64 volatile sstatus = r_sstatus();
			int ret = TCB::threadKill();
            __asm__ volatile("mv a0, %0\n" "sd a0, 10*8(fp)" :: "r"(ret): "a0", "memory");
            w_sepc(sepc);
			w_sstatus(sstatus);
            if(ret != -1){TCB::dispatch();}
		}
        else if(ins == 0x13){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if(ins == 0x21){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            sem_t* volatile handle;
            int volatile init;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            __asm__ volatile("mv %0, a2":"=r"(init));
            *handle = new Semaphore(init);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x23){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            sem_t id;
            __asm__ volatile("mv %0, a1":"=r"(id));
            id->wait();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x24){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            sem_t id;
            __asm__ volatile("mv %0, a1":"=r"(id));
            id->signal();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x25){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            sem_t id;
            time_t timeout;
            __asm__ volatile("mv %0, a1":"=r"(id));
            __asm__ volatile("mv %0, a2":"=r"(timeout));
            PeriodicThread* tmp = (PeriodicThread*) TCB::running;
            tmp->setPeriod(timeout);
            id->wait();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x26){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            sem_t id;
            int i = 1;
            __asm__ volatile("mv %0, a1":"=r"(id));
            __asm__ volatile("mv a1, %0"::"r"(i):);
            id->wait();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x31){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            time_t timeout;
            __asm__ volatile("mv %0, a1":"=r"(timeout));
            PeriodicThread* tmp = (PeriodicThread*) TCB::running;
            tmp->setPeriod(timeout);
            PeriodicThread::put();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x41){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            char c;
            __asm__ volatile("mv %0, a1":"=r"(c));
            Console::putc(c);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x42){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            Riscv::ms_sstatus(SSTATUS_SIE);
            Riscv::mc_sie(SIP_SSIP);
            Riscv::mc_sie(SIP_STIP);
            Riscv::ms_sie(SIP_SEIP);
            TCB::create_getc();

            w_sstatus(sstatus);
            w_sepc(sepc);
            //Riscv::ms_sstatus(SSTATUS_SIE);
        }
        else if(ins == 0x51){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            const char* string;
            __asm__ volatile("mv %0, a1":"=r"(string));
            printString(string);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(ins == 0x52){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            uint64 string;
            __asm__ volatile("mv %0, a1":"=r"(string));
            printInteger(string);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        Semaphore::check();
        PeriodicThread::check();
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
        //volatile uint64 *uart =  (volatile uint64*)CONSOLE_STATUS;
        volatile uint64 *uart1 =  (volatile uint64*)CONSOLE_TX_DATA;
        //volatile uint64 *uart2 =  (volatile uint64*)CONSOLE_RX_DATA;
        uint64 a = *uart1;
        uint64 x;
        __asm__ volatile("mv %0, a1":"=r"(x));
        //putc(a);
        if (x == 0x10){

            putc(a);
			TCB *old = TCB::running;
    		TCB::running = Scheduler::get();
			TCB::contextSwitch(&old->context, &TCB::running->context);

			delete old;
            Riscv::ms_sie(SIP_SSIP);
            Riscv::ms_sie(SIP_STIP);
        }

        console_handler();
    }
    else
    {
        // unexpected trap cause

    }
}