#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#define PIN_LED_BLUE 12
#define PIN_LED_RED 13

const uint pin_led_blue = PIN_LED_BLUE;
const uint pin_led_red = PIN_LED_RED;

void setup()
{
    stdio_init_all();
    gpio_init(PIN_LED_BLUE);
    gpio_set_dir(PIN_LED_BLUE, GPIO_OUT);
    gpio_init(PIN_LED_RED);
    gpio_set_dir(PIN_LED_RED, GPIO_OUT);
}

void vBlinkTask(void *pvParameters)
{
    // Type cast the parameter to uint pointer
    uint *ledPin = (uint *)pvParameters;

    while (true)
    {
        gpio_put(*ledPin, 1);           // Turn on LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500 ms
        gpio_put(*ledPin, 0);           // Turn off LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500 ms
        printf("Blinking LED\n");
    }
}

int main()
{
    setup();
    xTaskCreate(vBlinkTask, "Blue Blink Task", 128, (void *)&pin_led_blue, 1, NULL);
    xTaskCreate(vBlinkTask, "Red Blink Task", 128, (void *)&pin_led_red, 1, NULL);
    vTaskStartScheduler(); // Start the scheduler

    while (1)
    {
        printf("Main loop\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}
