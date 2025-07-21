#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#include "../h/Console.hpp"
#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/syscall_c.hpp"

// Thread Control Block
class TCB
{
public:
    virtual ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* arg);

    virtual bool isPeriodic() const { return false;}

    static void yield();

    static TCB *running;

    static int threadKill();
    static void create_getc();
protected:
    TCB(Body body, uint64 timeSlice, void* arg) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            arg(arg)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }
private:
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    void* arg;
    friend class Riscv;
    friend class Semaphore;
    friend class PeriodicThread;
    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
