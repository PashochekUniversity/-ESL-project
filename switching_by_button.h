#ifndef SWITCHING_BY_BUTTON_H
#define SWITCHING_BY_BUTTON_H

#include <stdbool.h>

void button_init(void);
void button_check_timeout(void);
bool button_is_playing(void);

#endif // SWITCHING_BY_BUTTON_H