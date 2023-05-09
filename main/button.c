#include "esp_log.h"
#include "iot_button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "RECEIVER"

#define GPIO_INPUT_PIN_SEL 1ULL << 0
#define ESP_INTR_FLAG_DEFAULT 0

void config_button_and_register_callbacks(void *bt_single_cb, void *bt_long_cb)
{
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 5000,
        .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
        .gpio_button_config = {
            .gpio_num = 0,
            .active_level = 0,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if (NULL == gpio_btn)
    {
        ESP_LOGE(TAG, "Button create failed");
    }

    if (bt_single_cb != NULL)
    {
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, bt_single_cb, NULL);
    }

    if (bt_long_cb != NULL)
    {
        iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, bt_long_cb, NULL);
    }
}