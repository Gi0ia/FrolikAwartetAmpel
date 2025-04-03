#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GPIO.hpp"  // Beispielhafter C++ Wrapper zur GPIO-Steuerung

// Fehlerflag (initialisiert mit aktuellem Zustand des Error-Pins)
volatile bool errorFlag = gpio_get(17);

// GPIO-Pins für die Ampel
static pico_cpp::GPIO_Pin redPin(2, pico_cpp::PinType::Output);
static pico_cpp::GPIO_Pin greenPin(3, pico_cpp::PinType::Output);
static pico_cpp::GPIO_Pin yellowPin(4, pico_cpp::PinType::Output);

// Task-Handles
TaskHandle_t xGreenTask       = NULL;
TaskHandle_t xGreenBlinkTask  = NULL;
TaskHandle_t xYellowTask      = NULL;
TaskHandle_t xRedTask         = NULL;
TaskHandle_t xRedYellowTask   = NULL;
TaskHandle_t xErrorTask       = NULL;

// Zeitdefinitionen
#define PHASE_DURATION pdMS_TO_TICKS(4000)
#define BLINK_INTERVAL pdMS_TO_TICKS(500)

// ---------------------- Ampel-Tasks ----------------------

void vGreenTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (errorFlag) continue;
        vTaskPrioritySet(NULL, 3);
        greenPin.set_high();
        redPin.set_low();
        yellowPin.set_low();
        vTaskDelay(PHASE_DURATION);
        greenPin.set_low();
        vTaskPrioritySet(xGreenBlinkTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xGreenBlinkTask);
    }
}

void vGreenBlinkTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (errorFlag) continue;
        vTaskPrioritySet(NULL, 3);
        TickType_t startTime = xTaskGetTickCount();
        while ((xTaskGetTickCount() - startTime) < PHASE_DURATION) {
            greenPin.set_high();
            vTaskDelay(BLINK_INTERVAL);
            greenPin.set_low();
            vTaskDelay(BLINK_INTERVAL);
        }
        vTaskPrioritySet(xYellowTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xYellowTask);
    }
}

void vYellowTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (errorFlag) continue;
        vTaskPrioritySet(NULL, 3);
        yellowPin.set_high();
        redPin.set_low();
        greenPin.set_low();
        vTaskDelay(PHASE_DURATION);
        yellowPin.set_low();
        vTaskPrioritySet(xRedTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xRedTask);
    }
}

void vRedTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (errorFlag) continue;
        vTaskPrioritySet(NULL, 3);
        redPin.set_high();
        greenPin.set_low();
        yellowPin.set_low();
        vTaskDelay(PHASE_DURATION);
        redPin.set_low();
        vTaskPrioritySet(xRedYellowTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xRedYellowTask);
    }
}

void vRedYellowTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (errorFlag) continue;
        vTaskPrioritySet(NULL, 3);
        redPin.set_high();
        yellowPin.set_high();
        greenPin.set_low();
        vTaskDelay(PHASE_DURATION);
        redPin.set_low();
        yellowPin.set_low();
        vTaskPrioritySet(xGreenTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xGreenTask);
    }
}

// ---------------------- Fehlerzustand ----------------------

void vErrorTask(void* pvParameters) {
    for (;;) {
        if (errorFlag) {
            TickType_t startTime = xTaskGetTickCount();
            while ((xTaskGetTickCount() - startTime) < PHASE_DURATION) {
                yellowPin.set_high();
                vTaskDelay(BLINK_INTERVAL);
                yellowPin.set_low();
                vTaskDelay(BLINK_INTERVAL);
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

// ---------------------- Main ----------------------

int main() {
    stdio_init_all();

    xTaskCreate(vGreenTask,       "GreenTask",       1024, NULL, 1, &xGreenTask);
    xTaskCreate(vGreenBlinkTask, "GreenBlinkTask",  1024, NULL, 1, &xGreenBlinkTask);
    xTaskCreate(vYellowTask,     "YellowTask",      1024, NULL, 1, &xYellowTask);
    xTaskCreate(vRedTask,        "RedTask",         1024, NULL, 2, &xRedTask);
    xTaskCreate(vRedYellowTask,  "RedYellowTask",   1024, NULL, 1, &xRedYellowTask);
    xTaskCreate(vErrorTask,      "ErrorTask",       1024, NULL, 4, &xErrorTask);

    xTaskNotifyGive(xRedTask);

    vTaskStartScheduler();

    while (true);  // sollte nie erreicht werden
    return 0;
}
