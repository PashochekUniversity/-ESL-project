#include "hsv_controller.h"
#include <stdint.h>
#include "hsv_color.h"
#include "pwm_leds.h"
#include "led_indicator.h"
#include "nrf_log.h"

#define HUE_STEP_DEG     1.8f
#define SV_STEP_PERCENT  1.0f

typedef enum
{
    HSV_MODE_IDLE = 0,
    HSV_MODE_HUE,
    HSV_MODE_SATURATION,
    HSV_MODE_VALUE
} hsv_mode_t;

static hsv_color_t m_color = { .hue = 10.8f, .sat = 100.0f, .val = 100.0f };
static hsv_mode_t  m_mode  = HSV_MODE_IDLE;

static float   m_sat_dir      = -1.0f;
static float   m_val_dir      = -1.0f;
static uint8_t m_log_throttle = 0;

static void apply_color_to_led2(void)
{
    rgb_duty_t rgb = hsv_to_rgb_duty(&m_color, PWM_LEDS_TOP_VALUE);
    pwm_leds_set_led2_rgb(rgb);
}

void hsv_controller_init(void)
{
    pwm_leds_init();
    led_indicator_init();
    apply_color_to_led2();
}

void hsv_controller_next_mode(void)
{
    switch (m_mode)
    {
        case HSV_MODE_IDLE:
        case HSV_MODE_VALUE:
            m_mode = HSV_MODE_HUE;
            led_indicator_set_pattern(LED_IND_BREATHE);
            NRF_LOG_INFO("Mode: HUE (H=%d.%d)",
                         (int)m_color.hue, (int)(m_color.hue * 10.0f) % 10);
            break;

        case HSV_MODE_HUE:
            m_mode = HSV_MODE_SATURATION;
            led_indicator_set_pattern(LED_IND_FAST_BLINK);
            NRF_LOG_INFO("Mode: SATURATION (S=%d%%)", (int)m_color.sat);
            break;

        case HSV_MODE_SATURATION:
            m_mode = HSV_MODE_VALUE;
            led_indicator_set_pattern(LED_IND_SOLID);
            NRF_LOG_INFO("Mode: VALUE (V=%d%%)", (int)m_color.val);
            break;
    }
}

void hsv_controller_adjust_step(void)
{
    if (m_mode == HSV_MODE_IDLE)
    {
        return;
    }

    if (m_mode == HSV_MODE_HUE)
    {
        hsv_step_hue(&m_color, HUE_STEP_DEG);
    }
    else if (m_mode == HSV_MODE_SATURATION)
    {
        hsv_step_bounce(&m_color.sat, &m_sat_dir, SV_STEP_PERCENT);
    }
    else if (m_mode == HSV_MODE_VALUE)
    {
        hsv_step_bounce(&m_color.val, &m_val_dir, SV_STEP_PERCENT);
    }

    apply_color_to_led2();

    m_log_throttle++;
    if (m_log_throttle >= 10)
    {
        m_log_throttle = 0;
        NRF_LOG_INFO("HSV -> H=%d.%d, S=%d%%, V=%d%%",
                     (int)m_color.hue, (int)(m_color.hue * 10.0f) % 10,
                     (int)m_color.sat, (int)m_color.val);
    }
}

void hsv_controller_on_hold_released(void)
{
    m_log_throttle = 0;
    if (m_mode != HSV_MODE_IDLE)
    {
        NRF_LOG_INFO("Saved HSV -> H=%d.%d, S=%d%%, V=%d%%",
                     (int)m_color.hue, (int)(m_color.hue * 10.0f) % 10,
                     (int)m_color.sat, (int)m_color.val);
    }
}