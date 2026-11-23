#include "pwm_leds.h"
#include "boards.h"
#include "nrfx_pwm.h"
#include "app_error.h"

static nrfx_pwm_t m_pwm = NRFX_PWM_INSTANCE(0);
static nrf_pwm_values_individual_t m_pwm_values;

void pwm_leds_init(void)
{
    nrfx_pwm_config_t pwm_config =
    {
        .output_pins =
        {
            LED_1 | NRFX_PWM_PIN_INVERTED,
            LED_2 | NRFX_PWM_PIN_INVERTED,
            LED_3 | NRFX_PWM_PIN_INVERTED,
            LED_4 | NRFX_PWM_PIN_INVERTED
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = PWM_LEDS_TOP_VALUE,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };

    ret_code_t err_code = nrfx_pwm_init(&m_pwm, &pwm_config, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_pwm_sequence_t const seq =
    {
        .values.p_individual = &m_pwm_values,
        .length              = NRF_PWM_VALUES_LENGTH(m_pwm_values),
        .repeats             = 0,
        .end_delay           = 0
    };

    (void)nrfx_pwm_simple_playback(&m_pwm, &seq, 1, NRFX_PWM_FLAG_LOOP);
}

void pwm_leds_set_led1(uint16_t brightness)
{
    if (brightness > PWM_LEDS_TOP_VALUE)
    {
        brightness = PWM_LEDS_TOP_VALUE;
    }
    m_pwm_values.channel_0 = brightness;
}

void pwm_leds_set_led2_rgb(rgb_duty_t rgb)
{
    m_pwm_values.channel_1 = rgb.r;
    m_pwm_values.channel_2 = rgb.g;
    m_pwm_values.channel_3 = rgb.b;
}