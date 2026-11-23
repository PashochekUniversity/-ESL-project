#ifndef HSV_COLOR_H
#define HSV_COLOR_H

#include <stdint.h>

typedef struct
{
    float hue; 
    float sat; 
    float val; 
} hsv_color_t;

typedef struct
{
    uint16_t r; 
    uint16_t g;
    uint16_t b;
} rgb_duty_t;

rgb_duty_t hsv_to_rgb_duty(const hsv_color_t * hsv, uint16_t max_duty);
void hsv_step_hue(hsv_color_t * hsv, float step_deg);
void hsv_step_bounce(float * param, float * direction, float step_percent);

#endif 