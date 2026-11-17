#include "switching_by_button.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrf_log.h"

#define PIN_BUTTON1 NRF_GPIO_PIN_MAP(1, 6)

extern volatile uint32_t sys_time_ms;

static volatile bool     is_playing      = false;
static volatile uint32_t last_press_time = 0;
static volatile uint8_t  click_count     = 0;

static void button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    uint32_t now = sys_time_ms;

    if ((now - last_press_time) < 50) {
        return;
    }

    if ((now - last_press_time) > 450) {
        click_count = 1;
    } else {
        click_count++;
    }

    last_press_time = now;

    if (click_count == 2) {
        is_playing = !is_playing;
        
        if (is_playing) {
            NRF_LOG_INFO("Work resumed: blinking started.");
        } else {
            NRF_LOG_INFO("Work paused: blinking frozen.");
        }
        
        click_count = 0;
    }
}

void button_init(void) {
    if (!nrfx_gpiote_is_init()) {
        nrfx_gpiote_init();
    }
    nrfx_gpiote_in_config_t btn_cfg = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    btn_cfg.pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(PIN_BUTTON1, &btn_cfg, button_handler);
    nrfx_gpiote_in_event_enable(PIN_BUTTON1, true);
}

void button_check_timeout(void) {
    if (click_count == 1 && ((sys_time_ms - last_press_time) > 450)) {
        click_count = 0;
    }
}

bool button_is_playing(void) {
    return is_playing;
}