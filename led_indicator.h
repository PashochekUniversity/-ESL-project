#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

typedef enum
{
    LED_IND_SOLID = 0,     
    LED_IND_BREATHE,       
    LED_IND_FAST_BLINK     
} led_ind_pattern_t;

void led_indicator_init(void);
void led_indicator_set_pattern(led_ind_pattern_t pattern);

#endif 