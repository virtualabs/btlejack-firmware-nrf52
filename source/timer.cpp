#include "timer.h"
#include "link.h"

/**
 * @brief Constructor
 * 
 * Instanciate a CustomTimer:
 * - associate a specific channel to this CustomTimer instance
 * - relies on a single NRFLowLevelTimer able to handle 3 dedicated channels.
 **/

CustomTimer::CustomTimer() {
  /* Instanciate NRFLowLevelTimer if required. */
  if (CustomTimer::m_timer == NULL)
  {
    /* Create our timer and enable it. */
    CustomTimer::m_timer = new NRFLowLevelTimer(NRF_TIMER0, TIMER0_IRQn);
    CustomTimer::m_timer->setMode(TimerMode::TimerModeTimer);
    CustomTimer::m_timer->timer_pointer = CustomTimer::timer_callback;
    CustomTimer::m_timer->setClockSpeed(1000); /* 1MHz, 1us period. */
    CustomTimer::m_timer->enable();
    CustomTimer::m_timer->enableIRQ();
  }

  /* Assign a channel to this timer. */
  m_channel = CustomTimer::m_current_channel;
  CustomTimer::m_current_channel++;
}

/**
 * @brief   Trigger a specific callback after a given period of time (in microseconds).
 *          Callback is only called once.
 * @param pfn_callback Callback function to be called.
 * @param period Delay in microseconds.
 **/

void CustomTimer::attach_us(FCallback pfn_callback, uint32_t period)
{
  /* Save period. */
  m_periods[m_channel] = period;

  /* Save callback. */
  CustomTimer::m_callbacks[m_channel] = pfn_callback;

  /* Save period. */
  CustomTimer::m_timer->setCompare(m_channel, CustomTimer::m_timer->captureCounter() + m_periods[m_channel]);
  //CustomTimer::m_timer->offsetCompare(m_channel, /*CustomTimer::m_timer->captureCounter() + */m_periods[m_channel]);
}


/**
 * @brief Get associated channel number.
 * @return channel number (0-2)
 **/

int CustomTimer::get_channel(void)
{
  return m_channel;
}


/**
 * @brief Rearm this timer (callback will be called once its delay passed).
 **/

void CustomTimer::rearm(void)
{
  CustomTimer::m_timer->offsetCompare(m_channel, m_periods[m_channel]);
}

/**
 * @brief Clear timer, it will not be triggered anymore.
 **/

void CustomTimer::detach(void)
{
  CustomTimer::m_timer->clearCompare(m_channel);
}

/**
 * @brief Internal callback used to dispatch timer interrupts.
 * 
 * @param channel_bitmsk Channel bitmask indicating triggered timer channels.
 **/

void CustomTimer::timer_callback(uint16_t channel_bitmsk)
{
  CustomTimer::m_timer->disableIRQ();
  for (int i=0; i<NB_CHANNELS_MAX; i++)
  {
    if (channel_bitmsk & (1<<i))
    {
      /* By default, clear timer. */
      CustomTimer::m_timer->clearCompare(i);

      /* Trigger callback. */
      CustomTimer::m_callbacks[i]();

      /* Rearm timer. */
      //CustomTimer::m_timer->offsetCompare(i, m_periods[i]);
    }
  }
  CustomTimer::m_timer->enableIRQ();
}

/* Static values. */
NRFLowLevelTimer *CustomTimer::m_timer = NULL;
int CustomTimer::m_current_channel = 0;
FCallback CustomTimer::m_callbacks[] = {NULL, NULL, NULL};
uint32_t CustomTimer::m_periods[] = {0, 0, 0};
