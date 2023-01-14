#include "led_handler.h"
#include <Arduino.h>


static bool timer_init = false;
static hw_timer_t* hw_timer;

led_t led_blue;
led_t led_red;
led_t* leds[] {
    &led_blue,
    &led_red
};
const size_t nbr_of_leds = (sizeof(leds) / sizeof(led_t*));


static inline void setLed(led_t* led, uint8_t value) {
    led->value = value;
    digitalWrite(led->pin, led->value);
}


void ARDUINO_ISR_ATTR timerInterrupt() {
    unsigned long now = millis();

    for (int i = 0; i < nbr_of_leds; i++) {
        led_t* led = leds[i];

        if (led->mode == LED_MODE_ON) {
            if (led->value != HIGH) {
                setLed(led, HIGH);
            }
        } else if (led->mode == LED_MODE_OFF) {
            if (led->value != LOW) {
                setLed(led, LOW);
            }
        } else {
            uint16_t delay = led->mode;
            if ((now - led->last_set) > delay) {
                setLed(led, !led->value);
                led->last_set = now;
            }
        }
    }

}


void led_init(led_t* led) {
    pinMode(led->pin, OUTPUT);
    led->mode = LED_MODE_OFF;
    setLed(led, LOW);

    // Start the led timer if not started yet.
    if (!timer_init) {
        timer_init = true;
        hw_timer = timerBegin(0, 80, true);
        timerAttachInterrupt(hw_timer, timerInterrupt, true);
        timerAlarmWrite(hw_timer, 10000, true);  // Every 10 ms (100 hz)
        timerAlarmEnable(hw_timer);
    }

}

void led_set(led_t* led, const led_mode_t mode) {
    led->mode = mode;
}


