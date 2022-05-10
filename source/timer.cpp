#include "timer.h"

CustomTicker::CustomTicker() {
}

void CustomTicker::onTick(MicroBitEvent e) {
    this->func();
}

void CustomTicker::attach(CallbackFunc callback, float s) {
    this->func = callback;
    this->interval = (s * 1000000.0f);
    uBit.messageBus.listen(MICROBIT_ID_MBED_TICKER, 0x0, this, &CustomTicker::onTick);
    system_timer_event_every_us(interval, MICROBIT_ID_MBED_TICKER, 0x0);
}
        
void CustomTicker::attach_us(CallbackFunc callback, int us) {
    this->func = callback;
    this->interval = us;

    uBit.messageBus.listen(MICROBIT_ID_MBED_TICKER, 0x0, this, &CustomTicker::onTick);
    system_timer_event_every_us(this->interval, MICROBIT_ID_MBED_TICKER, 0x0);
}

void CustomTicker::detach() {
    system_timer_cancel_event(MICROBIT_ID_MBED_TICKER, 0x0);
}
