#ifndef LED_HANDLER_H
#define LED_HANDLER_H

#include "stdint.h"


typedef enum {
    LED_MODE_OFF          = 0,
    LED_MODE_ON           = 1,
    LED_MODE_BLINK_0_5_HZ = 250,
    LED_MODE_BLINK_1_HZ   = 500,
    LED_MODE_BLINK_2_HZ   = 1000,
} led_mode_t;

typedef struct {
    uint8_t       pin;
    uint8_t       value;
    led_mode_t    mode;
    unsigned long last_set;
} led_t;


void led_init(led_t* led);

void led_set(led_t* led, const led_mode_t mode);


extern led_t led_blue;
extern led_t led_red;


#endif /* LED_HANDLER_H */
