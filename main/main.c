#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "blink_led.h"
#include "bluetooth.h"
#include "button.h"
#include "screen.h"

#define TRUE 1
#define FALSE 0

unsigned int SHOW_BPM = FALSE;
unsigned int SHOW_NUMBER = FALSE;
unsigned int BPM = 0;
unsigned int NUMBER = 0;

void button_callback()
{
    if (SHOW_BPM)
    {
        SHOW_BPM = FALSE;
        stop_blink_led();
        return;
    }

    if (SHOW_NUMBER)
    {
        SHOW_NUMBER = FALSE;
        clear_screen();
    }
}

void splitString(const char *original, char *firstPart, char *secondPart)
{
    const char *delimiter = ".";
    char copy[strlen(original) + 1];
    strcpy(copy, original);

    char *token = strtok(copy, delimiter);
    if (token != NULL)
    {
        strcpy(firstPart, token);
        token = strtok(NULL, delimiter);
        if (token != NULL)
        {
            strcpy(secondPart, token);
        }
        else
        {
            strcpy(secondPart, "");
        }
    }
    else
    {
        strcpy(firstPart, "");
        strcpy(secondPart, "");
    }
}

void bluetooth_callback(char *data, uint32_t len)
{
    ESP_LOGI("Bluetooth", "Received data: %.*s, Length: %u", (unsigned int)len, data, (unsigned int)len);

    char charArrayNumber[10];
    char charArrayBPM[10];
    splitString(data, charArrayNumber, charArrayBPM);

    BPM = atoi(charArrayBPM);
    NUMBER = atoi(charArrayNumber);
    
    if (SHOW_BPM) {
        stop_blink_led();
    }

    if (SHOW_NUMBER) {
        clear_screen();
    }

    start_blink_led(BPM);
    show_in_screen(NUMBER);
    SHOW_BPM = TRUE;
    SHOW_NUMBER = TRUE;
    return;
}

void app_main()
{
    start_bluetooth_register_callback(bluetooth_callback);
    config_button_and_register_callbacks(button_callback, NULL);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}