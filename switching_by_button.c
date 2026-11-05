#include "switching_by_button.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
const int array[4]={6,6,0,3};
const int array_leds[4]={LED1, LED_R,LED_G,LED_B};


void pin_settings(){
    nrf_gpio_cfg_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP);
    for(int i=0;i<4;i++){
        nrf_gpio_cfg_output(array_leds[i]);
        nrf_gpio_pin_set(array_leds[i]);
    }
}

void switch_leds(int i){
    for(int j=0;j<array[i];j++){
        while(READ_BUTTON()!=0){
            nrf_delay_ms(5);
        }
        nrf_gpio_pin_write(array_leds[i], READ_BUTTON());
        for(int k=0;k<1000;k++){
            nrf_delay_ms(1);
            while(READ_BUTTON()!=0){
                nrf_delay_ms(5);
            }
        }
        nrf_gpio_pin_set(array_leds[i]);
        nrf_delay_ms(500);
    }
}