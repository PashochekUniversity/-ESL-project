#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_clock.h"
#include "app_timer.h"
#include "nrf_log_backend_usb.h"
#include "app_usbd.h"
#include "hsv_controller.h"
#include "switching_by_button.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

static void lfclk_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
    while (!nrf_drv_clock_lfclk_is_running())
    {
    }
}

int main(void)
{
    lfclk_init();

    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    hsv_controller_init();
    button_init();

    NRF_LOG_INFO("HSV Picker started");

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();


        NRF_LOG_PROCESS();
    }
}