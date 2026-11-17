#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "switch_leds.h"
#include "switching_by_button.h"


volatile uint32_t sys_time_ms = 0;

int main(void) {
    leds_init();
    button_init();

    while (true) {
        button_check_timeout();
        leds_process(button_is_playing());
        
        nrf_delay_ms(10);
        sys_time_ms += 10;
    }
}