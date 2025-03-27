#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Definitions
#define BUTTON_READ_DELAY_MS 100
#define QUEUE_LENGTH 5

// Pin definitions
#define PIN_BUTTON_A 5
#define PIN_LED_BLUE 12

// Global variables
QueueHandle_t buttonQueue = NULL;
SemaphoreHandle_t ledSemaphore = NULL;
int buttonState = 0;
int ledState = 0;

// Prototypes
void buttonReadTask(void *pvParameters);
void buttonProcessTask(void *pvParameters);
void ledControlTask(void *pvParameters);

/** @brief Setup function to initialize GPIO pins. */
void setup()
{
    stdio_init_all();
    gpio_init(PIN_LED_BLUE);
    gpio_set_dir(PIN_LED_BLUE, GPIO_OUT);
    gpio_init(PIN_BUTTON_A);
    gpio_set_dir(PIN_BUTTON_A, GPIO_IN);
    gpio_pull_up(PIN_BUTTON_A);
}

/** @brief Main function to initialize FreeRTOS and create tasks. */
int main()
{
    setup();

    // Create Queue and Semaphore
    buttonQueue = xQueueCreate(QUEUE_LENGTH, sizeof(int));
    ledSemaphore = xSemaphoreCreateBinary();
    if (buttonQueue == NULL || ledSemaphore == NULL)
    {
        printf("Failed to create queue or semaphore\n");
        return 1;
    }

    // Create Tasks
    xTaskCreate(buttonReadTask, "Button Read Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(buttonProcessTask, "Button Process Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(ledControlTask, "Led Control Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

/** @brief Task to read button state and send it to the queue. */
void buttonReadTask(void *pvParameters)
{
    while (1)
    {
        // Read button state
        int buttonAState = gpio_get(PIN_BUTTON_A);

        // Send button state to queue
        if (xQueueSend(buttonQueue, &buttonAState, 0) != pdPASS)
        {
            printf("Failed to send button state to queue\n");
        }

        // Delay for a while
        vTaskDelay(pdMS_TO_TICKS(BUTTON_READ_DELAY_MS));
    }
}

/** @brief Task to process button state and signal LED control task. */
void buttonProcessTask(void *pvParameters)
{
    while (1)
    {
        // Wait for button state from queue
        if (xQueueReceive(buttonQueue, &buttonState, portMAX_DELAY) == pdPASS)
        {
            // Check if button is pressed (active low)
            if (buttonState == 0)
            {
                ledState = true;
            }
            else
            {
                ledState = false;
            }
            xSemaphoreGive(ledSemaphore); // Signal LED task
        }
    }
}

/** @brief Task to control LED based on button state. */
void ledControlTask(void *pvParameters)
{
    while (1)
    {
        // Wait for semaphore signal
        if (xSemaphoreTake(ledSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Toggle LED state
            gpio_put(PIN_LED_BLUE, ledState);
            printf("LED state: %d\n", ledState);
        }
    }
}