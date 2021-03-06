/**
 * Micro:Bit Custom Timer
 * 
 * This module provides a CustomTimer class based on nRF52's NRF_TIMER0
 * (Hardware timer).
 * 
 * When CustomTimer::attach_us() is called, it sets a callback function
 * that will be triggered once the specified delay passed. 
 * 
 * /!\/!\/!\/!\
 * 
 * Be careful: this callback is in charge of rearming the timer if required.
 * -------------------------------------------------------------------------
 *  
 **/

#ifndef __INC_TIMER_H
#define __INC_TIMER_H

#include "MicroBit.h"

#define NB_CHANNELS_MAX 3

/* Callback type. */
typedef void (*FCallback)(void);

/* uBit object from PXT or CODAL. 
 * TODO: MICROBIT_DAL_PXT will be added to PXT to replace this define */
#ifdef MICROBIT_DAL_FIBER_USER_DATA
#include "pxt.h"
#else
extern MicroBit uBit;
#endif

class CustomTimer {
  
  private:
    static void timer_callback(uint16_t channel_bitmsk);
    int m_channel;

    static int m_current_channel;
    static FCallback m_callbacks[NB_CHANNELS_MAX];
    static uint32_t m_periods[NB_CHANNELS_MAX];
    static NRFLowLevelTimer *m_timer;

  public:
      CustomTimer();
      int get_channel(void);
            
      /* Attach a callback to this timer that will be triggered after <period> microseconds. */
      void attach_us(FCallback pfn_callback, uint32_t period);

      /* Rearm timer. */
      void rearm(void);

      /* Detach callback. */
      void detach(void);
};

#endif /* __INC_TIMER_H */