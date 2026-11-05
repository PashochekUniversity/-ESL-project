
/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
//#include "switch_leds.h"
#include"switching_by_button.h"

/**
 * @brief Function for application main entry.
 */
int main(void)
{  
    pin_settings();
    while(true){
        for(int i=0;i<4;i++){
            switch_leds(i);
        }
    }
}

