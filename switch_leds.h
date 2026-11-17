#ifndef SWITCH_LEDS_H
#define SWITCH_LEDS_H

#include <stdbool.h>

void leds_init(void);
void leds_process(bool is_playing);

#endif // SWITCH_LEDS_H