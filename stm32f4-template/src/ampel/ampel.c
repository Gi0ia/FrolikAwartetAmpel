#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GPIO.hpp"  // Beispielhafter C++ Wrapper zur GPIO-Steuerung

// Globale Fehlerflag. Wird extern gesetzt, wenn ein Fehler (z. B. Kommunikationsfehler) vorliegt.
volatile bool errorFlag = gpio_get(17);

// Definieren Sie drei GPIO-Pins: einer für Rot, einer für Grün, einer für Gelb.
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

// Zeitdefinitionen: Alle Phasen laufen 4 Sekunden lang
#define PHASE_DURATION pdMS_TO_TICKS(4000)
// Blinkintervall für die LED: 0.5 s
#define BLINK_INTERVAL pdMS_TO_TICKS(500)

// -----------------------------------------------------------------------------
// Task: Grün
// -----------------------------------------------------------------------------
void vGreenTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(errorFlag) continue;
        // Setze die Priority auf 3, umd den Task auf Working zu setzen
        vTaskPrioritySet(NULL, 3);
        printf("Phase: GRÜN\n");
        // Setze: Grün an, Rot & Gelb aus
        greenPin.set_high();
        redPin.set_low();
        yellowPin.set_low();
        vTaskDelay(PHASE_DURATION);
        greenPin.set_low();
        // Nächste Phase: Grün blinkend
        // Setze die Priority auf 1 und die des GreenBlinkTask auf 2
        vTaskPrioritySet(xGreenBlinkTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xGreenBlinkTask);
    }
}

// -----------------------------------------------------------------------------
// Task: Grün blinkend
// -----------------------------------------------------------------------------
void vGreenBlinkTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(errorFlag) continue;
        // Setze die Priority auf 3, umd den Task auf Working zu setzen
        vTaskPrioritySet(NULL, 3);
        printf("Phase: GRÜN BLINKEND\n");
        TickType_t startTime = xTaskGetTickCount();
        // Innerhalb der 4 Sekunden blinkt die grüne LED mit BLINK_INTERVAL
        while ((xTaskGetTickCount() - startTime) < PHASE_DURATION) {
            greenPin.set_high();
            printf("now green");
            vTaskDelay(BLINK_INTERVAL);
            greenPin.set_low();
            printf("now off");
            vTaskDelay(BLINK_INTERVAL);
        }
        // Nächste Phase: Gelb
        // Setze die Priority auf 1 und die des YellowTask auf 2
        vTaskPrioritySet(xYellowTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xYellowTask);
    }
}

// -----------------------------------------------------------------------------
// Task: Gelb
// -----------------------------------------------------------------------------
void vYellowTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(errorFlag) continue;
        // Setze die Priority auf 3, umd den Task auf Working zu setzen
        vTaskPrioritySet(NULL, 3);
        printf("Phase: GELB\n");
        yellowPin.set_high();
        redPin.set_low();
        greenPin.set_low();
        vTaskDelay(PHASE_DURATION);
        yellowPin.set_low();
        // Nächste Phase: Rot
        // Setze die Priority auf 1 und die des RedTask auf 2
        vTaskPrioritySet(xRedTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xRedTask);
    }
}

// -----------------------------------------------------------------------------
// Task: Rot
// -----------------------------------------------------------------------------
void vRedTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(errorFlag) continue;
        // Setze die Priority auf 3, umd den Task auf Working zu setzen
        vTaskPrioritySet(NULL, 3);
        printf("Phase: ROT\n");
        redPin.set_high();
        greenPin.set_low();
        yellowPin.set_low();
        vTaskDelay(PHASE_DURATION);
        redPin.set_low();
        // Nächste Phase: Rot-Gelb
        // Setze die Priority auf 1 und die des RedYellowTask auf 2
        vTaskPrioritySet(xRedYellowTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xRedYellowTask);
    }
}

// -----------------------------------------------------------------------------
// Task: Rot-Gelb
// -----------------------------------------------------------------------------
void vRedYellowTask(void* pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(errorFlag) continue;
        // Setze die Priority auf 3, umd den Task auf Working zu setzen
        vTaskPrioritySet(NULL, 3);
        printf("Phase: ROT-GELB\n");
        redPin.set_high();
        yellowPin.set_high();
        greenPin.set_low();
        vTaskDelay(PHASE_DURATION);
        redPin.set_low();
        yellowPin.set_low();
        // Nächste Phase: Grün
        // Setze die Priority auf 1 und die des GreenTask auf 2
        vTaskPrioritySet(xGreenTask, 2);
        vTaskPrioritySet(NULL, 1);
        xTaskNotifyGive(xGreenTask);
    }
}

// -----------------------------------------------------------------------------
// Task: Fehlerzustand (Gelb blinkend)
// -----------------------------------------------------------------------------
void vErrorTask(void* pvParameters) {
    for (;;) {
        if (errorFlag) {
            printf("ERROR: GELB BLINKEND (Fehlerzustand)\n");
            TickType_t startTime = xTaskGetTickCount();
            // Innerhalb von 4 Sekunden blinkt gelb
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

// -----------------------------------------------------------------------------
// Main-Funktion: Aufgaben erstellen und Scheduler starten
// -----------------------------------------------------------------------------
int main() {
    stdio_init_all();

    /* Erstellen der Tasks mit unterschiedlichen Prioritäten:
     * (Prioritäten: suspended = 1, ready = 2, suspend = 3, danger = 4)
     */
    xTaskCreate(vGreenTask, "GreenTask", 1024, NULL, 1, &xGreenTask);
    xTaskCreate(vGreenBlinkTask, "GreenBlinkTask", 1024, NULL, 1, &xGreenBlinkTask);
    xTaskCreate(vYellowTask, "YellowTask", 1024, NULL, 1, &xYellowTask);
    xTaskCreate(vRedTask, "RedTask", 1024, NULL, 2, &xRedTask);
    xTaskCreate(vRedYellowTask, "RedYellowTask", 1024, NULL, 1, &xRedYellowTask);
    xTaskCreate(vErrorTask, "ErrorTask", 1024, NULL, 4, &xErrorTask);

    // Starten des Zyklus: Zunächst wird der Grün-Task aktiviert.
    xTaskNotifyGive(xRedTask);

    // Starten des FreeRTOS-Schedulers
    vTaskStartScheduler();

    // Dieser Punkt sollte niemals erreicht werden, wenn der Scheduler korrekt läuft.
    for(;;);
    return 0;
}