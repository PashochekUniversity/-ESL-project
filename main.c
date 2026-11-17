#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "app_usbd.h"
#include "switch_leds.h"
#include "switching_by_button.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

volatile uint32_t sys_time_ms = 0;

static void lfclk_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
    while (!nrf_drv_clock_lfclk_is_running())
    {
    }
}

int main(void) {
    lfclk_init();
    
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    leds_init();
    button_init();

    NRF_LOG_INFO("System started. Double-click button to start/pause.");

    while (true) {
        while (app_usbd_event_queue_process())
        {
            // Обработка событий USB-стека
        }

        button_check_timeout();
        leds_process(button_is_playing());
        
        NRF_LOG_PROCESS();

        nrf_delay_ms(10);
        sys_time_ms += 10;
    }
}