#ifndef PWM_LEDS_H
#define PWM_LEDS_H

#include <stdint.h>
#include "hsv_color.h"

#define PWM_LEDS_TOP_VALUE 1000U

void pwm_leds_init(void);
void pwm_leds_set_led1(uint16_t brightness);
void pwm_leds_set_led2_rgb(rgb_duty_t rgb);

#endif 