#ifndef SWITCH_BY_BUTTON_H
#define SWITCH_BY_BUTTON_H
#endif
#define BUTTON_PIN 38 
#define LED1 6
#define LED_R 8
#define LED_G 41
#define LED_B 12
#define READ_BUTTON() nrf_gpio_pin_read(BUTTON_PIN)
void pin_settings();
void switch_leds(int i);