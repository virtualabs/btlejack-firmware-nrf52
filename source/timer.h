#ifndef __INC_TIMER_H
#define __INC_TIMER_H

#include "MicroBit.h"

#define NB_CHANNELS_MAX 3

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
    
    static int m_current_channel;
    static FCallback m_callbacks[NB_CHANNELS_MAX];
    static uint32_t m_periods[NB_CHANNELS_MAX];

    static void timer_callback(uint16_t channel_bitmsk);

    int m_channel;

  public:
      static NRFLowLevelTimer *m_timer;
      CustomTimer();
      void attach_us(FCallback pfn_callback, uint32_t period);
      void detach(void);
};

#endif /* __INC_TIMER_H */