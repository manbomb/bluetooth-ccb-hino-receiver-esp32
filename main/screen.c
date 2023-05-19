#include "esp_log.h"

void show_in_screen(int number)
{
    ESP_LOGI("screen", "HINO: %i", number);
}

void clear_screen()
{
    ESP_LOGI("screen", "clear");
}