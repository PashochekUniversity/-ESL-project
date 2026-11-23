#include "led_indicator.h"
#include <stdint.h>
#include "app_timer.h"
#include "app_error.h"
#include "pwm_leds.h"

#define ANIM_INTERVAL_MS  20U
#define BREATHE_STEP      25
#define FAST_BLINK_TICKS  5U

APP_TIMER_DEF(m_anim_timer);

static led_ind_pattern_t m_pattern  = LED_IND_SOLID;
static uint16_t          m_duty     = PWM_LEDS_TOP_VALUE;
static int16_t           m_fade_dir = -BREATHE_STEP;
static uint8_t           m_tick_cnt = 0;

static void anim_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    switch (m_pattern)
    {
        case LED_IND_SOLID:
            m_duty = PWM_LEDS_TOP_VALUE;
            pwm_leds_set_led1(m_duty);
            break;

        case LED_IND_BREATHE:
        {
            int32_t next_duty = (int32_t)m_duty + m_fade_dir;
            if (next_duty >= (int32_t)PWM_LEDS_TOP_VALUE)
            {
                next_duty  = PWM_LEDS_TOP_VALUE;
                m_fade_dir = -BREATHE_STEP;
            }
            else if (next_duty <= 0)
            {
                next_duty  = 0;
                m_fade_dir = BREATHE_STEP;
            }
            m_duty = (uint16_t)next_duty;
            pwm_leds_set_led1(m_duty);
            break;
        }

        case LED_IND_FAST_BLINK:
            m_tick_cnt++;
            if (m_tick_cnt >= FAST_BLINK_TICKS)
            {
                m_tick_cnt = 0;
                m_duty     = (m_duty == 0) ? PWM_LEDS_TOP_VALUE : 0;
                pwm_leds_set_led1(m_duty);
            }
            break;
    }
}

void led_indicator_init(void)
{
    m_pattern = LED_IND_SOLID;
    m_duty    = PWM_LEDS_TOP_VALUE;
    pwm_leds_set_led1(m_duty);

    ret_code_t err_code = app_timer_create(&m_anim_timer,
                                           APP_TIMER_MODE_REPEATED,
                                           anim_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_anim_timer,
                               APP_TIMER_TICKS(ANIM_INTERVAL_MS),
                               NULL);
    APP_ERROR_CHECK(err_code);
}

void led_indicator_set_pattern(led_ind_pattern_t pattern)
{
    m_pattern  = pattern;
    m_tick_cnt = 0;
    m_duty     = PWM_LEDS_TOP_VALUE;
    m_fade_dir = -BREATHE_STEP;
    pwm_leds_set_led1(m_duty);
}