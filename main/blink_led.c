#include <stddef.h>
#include "bluetooth.h"
#include "button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define MIN_CYCLE_TIME (1000 / 240)
#define MAX_CYCLE_TIME (1000 / 30)

TaskHandle_t task_handle;
int bpm = 120;

static void blink_led(int duration_ms)
{
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    gpio_set_level(LED_PIN, 0);
}

static void led_bpm_task(void *arg)
{
    ESP_LOGI("BPM", "%i", bpm);
    int cycle_time_ms = 60000 / bpm;
    ESP_LOGI("cycle_time_ms", "%i", cycle_time_ms);
    while (1)
    {
        blink_led(cycle_time_ms / 2);
        vTaskDelay(pdMS_TO_TICKS(cycle_time_ms / 2));
    }
}

void start_blink_led(int bpmParam)
{
    bpm = bpmParam;
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    xTaskCreate(led_bpm_task, "led_bpm_task", 2048, &bpm, 4, &task_handle);
}

void stop_blink_led()
{
    vTaskDelete(task_handle);
    gpio_set_level(LED_PIN, 0);
}