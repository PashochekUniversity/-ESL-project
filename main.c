
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
#include "nrf_delay.h"
//#include "switch_leds.h"
#include "nrf_gpio.h"
#define BUTTON_PIN 38 
#define LED1 6
#define LED_R 8
#define LED_G 41
#define LED_B 12
#define READ_BUTTON() nrf_gpio_pin_read(BUTTON_PIN)

const int array_leds[4]={LED1, LED_R,LED_G,LED_B};
void pin_settings(){
    nrf_gpio_cfg_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP);
    for(int i=0;i<4;i++){
        nrf_gpio_cfg_output(array_leds[i]);
        nrf_gpio_pin_set(array_leds[i]);
    }
}

const int array[4]={6,6,0,3};

void switch_leds(int i){
    for(int j=0;j<array[i];j++){
        if(READ_BUTTON()==0){
            nrf_gpio_pin_write(array_leds[i], READ_BUTTON());
            nrf_delay_ms(1000);
            nrf_gpio_pin_set(array_leds[i]);
            nrf_delay_ms(500);
        }
    }
}
/**
 * @brief Function for application main entry.
 */
int main(void)
{
    
    /* Configure board. */
    

    /* Toggle LEDs. */
   
    pin_settings();
    while(true){
        for(int i=0;i<4;i++){
            switch_leds(i);
            
        }
    }
}

