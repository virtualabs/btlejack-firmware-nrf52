#include "timer.h"

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

void CustomTimer::attach_us(FCallback pfn_callback, uint32_t period)
{
  /* Save period. */
  m_periods[m_channel] = period;

  /* Save callback. */
  CustomTimer::m_callbacks[m_channel] = pfn_callback;

  /* Save period. */
  CustomTimer::m_timer->offsetCompare(m_channel, m_periods[m_channel]);
}

void CustomTimer::detach(void)
{
  CustomTimer::m_timer->clearCompare(m_channel);
}

void CustomTimer::timer_callback(uint16_t channel_bitmsk)
{
  for (int i=0; i<NB_CHANNELS_MAX; i++)
  {
    if (channel_bitmsk & (1<<i))
    {
      /* Trigger callback. */
      CustomTimer::m_callbacks[i]();

      /* Rearm timer. */
      CustomTimer::m_timer->clearCompare(i);
      CustomTimer::m_timer->offsetCompare(i, m_periods[i]);
    }
  }

}

NRFLowLevelTimer *CustomTimer::m_timer = NULL;
int CustomTimer::m_current_channel = 0;
FCallback CustomTimer::m_callbacks[] = {NULL, NULL, NULL};
uint32_t CustomTimer::m_periods[] = {0, 0, 0};
