#ifndef PERIODICTHREAD_HPP
#define PERIODICTHREAD_HPP
#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/list.hpp"


class PeriodicThread: public TCB{

public:
    static List<PeriodicThread> sleepingThreadsQueue;

public:
    void terminate();
    virtual bool isPeriodic() const override {return true;}
    time_t getPeriod() const;
    void setPeriod(time_t period);
    static PeriodicThread* createPeriodicThread(time_t period,Body body, void* arg);
    virtual ~PeriodicThread();
    static void check();
    static void put();

protected:
    using TCB::TCB;
    //PeriodicThread(time_t period);
    //virtual void periodicActivation();
    PeriodicThread(time_t period,Body body, uint64 timeSlice, void* arg):TCB(body,timeSlice,arg),period(period){}
private:
    time_t period;
};

#endif //PERIODICTHREAD_HPP
