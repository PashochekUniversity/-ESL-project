#include "switching_by_button.h"
#include <stdbool.h>
#include "boards.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "app_timer.h"
#include "app_error.h"
#include "hsv_controller.h"

#define DEBOUNCE_DELAY_MS       30U
#define DOUBLE_CLICK_WINDOW_MS  350U
#define HOLD_REPEAT_INTERVAL_MS 25U

typedef enum
{
    BTN_FSM_IDLE = 0,
    BTN_FSM_FIRST_DOWN,
    BTN_FSM_WAIT_SECOND_PRESS,
    BTN_FSM_SECOND_DOWN,
    BTN_FSM_HOLDING
} btn_fsm_state_t;

APP_TIMER_DEF(m_debounce_timer);
APP_TIMER_DEF(m_dblclick_timer);
APP_TIMER_DEF(m_hold_timer);

static btn_fsm_state_t m_fsm_state  = BTN_FSM_IDLE;
static bool            m_is_pressed = false;

static bool is_button_physically_pressed(void)
{
    return (nrf_gpio_pin_read(BUTTON_1) == BUTTONS_ACTIVE_STATE);
}

static void on_clean_button_press(void)
{
    switch (m_fsm_state)
    {
        case BTN_FSM_IDLE:
            m_fsm_state = BTN_FSM_FIRST_DOWN;
            (void)app_timer_stop(m_dblclick_timer);
            (void)app_timer_start(m_dblclick_timer,
                                  APP_TIMER_TICKS(DOUBLE_CLICK_WINDOW_MS),
                                  NULL);
            break;

        case BTN_FSM_WAIT_SECOND_PRESS:
            (void)app_timer_stop(m_dblclick_timer);
            m_fsm_state = BTN_FSM_SECOND_DOWN;
            hsv_controller_next_mode();
            break;

        default:
            break;
    }
}

static void on_clean_button_release(void)
{
    switch (m_fsm_state)
    {
        case BTN_FSM_FIRST_DOWN:
            m_fsm_state = BTN_FSM_WAIT_SECOND_PRESS;
            (void)app_timer_stop(m_dblclick_timer);
            (void)app_timer_start(m_dblclick_timer,
                                  APP_TIMER_TICKS(DOUBLE_CLICK_WINDOW_MS),
                                  NULL);
            break;

        case BTN_FSM_SECOND_DOWN:
            m_fsm_state = BTN_FSM_IDLE;
            break;

        case BTN_FSM_HOLDING:
            (void)app_timer_stop(m_hold_timer);
            m_fsm_state = BTN_FSM_IDLE;
            hsv_controller_on_hold_released();
            break;

        default:
            m_fsm_state = BTN_FSM_IDLE;
            break;
    }
}

static void debounce_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    bool current_state = is_button_physically_pressed();
    if (current_state != m_is_pressed)
    {
        m_is_pressed = current_state;
        if (m_is_pressed)
        {
            on_clean_button_press();
        }
        else
        {
            on_clean_button_release();
        }
    }
}

static void dblclick_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    if (m_fsm_state == BTN_FSM_FIRST_DOWN)
    {
        m_fsm_state = BTN_FSM_HOLDING;
        hsv_controller_adjust_step();
        (void)app_timer_start(m_hold_timer,
                              APP_TIMER_TICKS(HOLD_REPEAT_INTERVAL_MS),
                              NULL);
    }
    else if (m_fsm_state == BTN_FSM_WAIT_SECOND_PRESS)
    {
        m_fsm_state = BTN_FSM_IDLE;
    }
}

static void hold_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    if (m_fsm_state == BTN_FSM_HOLDING)
    {
        hsv_controller_adjust_step();
    }
}

static void gpiote_pin_event_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    UNUSED_PARAMETER(pin);
    UNUSED_PARAMETER(action);

    (void)app_timer_stop(m_debounce_timer);
    (void)app_timer_start(m_debounce_timer,
                          APP_TIMER_TICKS(DEBOUNCE_DELAY_MS),
                          NULL);
}

void button_init(void)
{
    ret_code_t err_code;

    if (!nrfx_gpiote_is_init())
    {
        err_code = nrfx_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }

    err_code = app_timer_create(&m_debounce_timer,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                debounce_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_dblclick_timer,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                dblclick_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_hold_timer,
                                APP_TIMER_MODE_REPEATED,
                                hold_timeout_handler);
    APP_ERROR_CHECK(err_code);

    nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = BUTTON_PULL;

    err_code = nrfx_gpiote_in_init(BUTTON_1, &in_config, gpiote_pin_event_handler);
    APP_ERROR_CHECK(err_code);

    nrfx_gpiote_in_event_enable(BUTTON_1, true);
}