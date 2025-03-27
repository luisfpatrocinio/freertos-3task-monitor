#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#define PIN_LED 12

void vBlinkTask()
{
    while (true)
    {
        gpio_put(PIN_LED, 1);           // Turn on LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500 ms
        gpio_put(PIN_LED, 0);           // Turn off LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500 ms
        printf("Blinking LED\n");
    }
}

int main()
{
    stdio_init_all();
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    xTaskCreate(vBlinkTask, "Blink Task", 128, NULL, 1, NULL);
    vTaskStartScheduler(); // Start the scheduler


    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
