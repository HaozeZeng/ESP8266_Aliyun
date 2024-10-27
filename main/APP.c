#include "APP.h"

void LED_Init()
{
    gpio_config_t led_cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1 << GPIO_NUM_5), // D1
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&led_cfg);
    gpio_set_level(GPIO_NUM_5, 0);
}