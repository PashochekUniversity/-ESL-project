#include "hsv_color.h"
#include <math.h>

rgb_duty_t hsv_to_rgb_duty(const hsv_color_t * hsv, uint16_t max_duty)
{
    float s = hsv->sat / 100.0f;
    float v = hsv->val / 100.0f;

    float c = v * s;
    float h_prime = fmodf(hsv->hue / 60.0f, 6.0f);
    if (h_prime < 0.0f)
    {
        h_prime += 6.0f;
    }

    float x = c * (1.0f - fabsf(fmodf(h_prime, 2.0f) - 1.0f));
    float m = v - c;

    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f;

    if (h_prime < 1.0f)      { r1 = c; g1 = x; b1 = 0.0f; }
    else if (h_prime < 2.0f) { r1 = x; g1 = c; b1 = 0.0f; }
    else if (h_prime < 3.0f) { r1 = 0.0f; g1 = c; b1 = x; }
    else if (h_prime < 4.0f) { r1 = 0.0f; g1 = x; b1 = c; }
    else if (h_prime < 5.0f) { r1 = x; g1 = 0.0f; b1 = c; }
    else                     { r1 = c; g1 = 0.0f; b1 = x; }

    rgb_duty_t out;
    out.r = (uint16_t)((r1 + m) * (float)max_duty + 0.5f);
    out.g = (uint16_t)((g1 + m) * (float)max_duty + 0.5f);
    out.b = (uint16_t)((b1 + m) * (float)max_duty + 0.5f);

    if (out.r > max_duty) out.r = max_duty;
    if (out.g > max_duty) out.g = max_duty;
    if (out.b > max_duty) out.b = max_duty;

    return out;
}

void hsv_step_hue(hsv_color_t * hsv, float step_deg)
{
    hsv->hue += step_deg;
    if (hsv->hue >= 360.0f)
    {
        hsv->hue -= 360.0f;
    }
}

void hsv_step_bounce(float * param, float * direction, float step_percent)
{
    *param += (*direction) * step_percent;
    if (*param >= 100.0f)
    {
        *param     = 100.0f;
        *direction = -1.0f;
    }
    else if (*param <= 0.0f)
    {
        *param     = 0.0f;
        *direction = 1.0f;
    }
}