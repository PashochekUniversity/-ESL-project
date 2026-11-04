void leds_switch(int i){
    const int array[LEDS_NUMBER]={6,6,0,3};
    for(int j=0;j<array[i];j++){
    bsp_board_led_invert(i);
    nrf_delay_ms(500);
    bsp_board_led_invert(i);
    nrf_delay_ms(500);
    }
 }