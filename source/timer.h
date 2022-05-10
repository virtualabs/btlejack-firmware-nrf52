#ifndef __INC_TIMER_H
#define __INC_TIMER_H

#include "MicroBit.h"

/* uBit object from PXT or CODAL. 
 * TODO: MICROBIT_DAL_PXT will be added to PXT to replace this define */
#ifdef MICROBIT_DAL_FIBER_USER_DATA
#include "pxt.h"
#else
extern MicroBit uBit;
#endif

#include "MicroBitCompat.h"
#include "Timer.h"
#include "MicroBitEvent.h"

typedef void (*CallbackFunc)(void);

class CustomTicker {
    private:
        uint32_t interval;
        CallbackFunc func;

    public:

        CustomTicker();
        void onTick(MicroBitEvent e);
        void attach(CallbackFunc callback, float s);        
        void attach_us(CallbackFunc callback, int us);
        void detach();
};

#endif /* __INC_TIMER_H */