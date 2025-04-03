#include "traffic_light_control.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

volatile uint32_t lastSPISignalTick = 0;

void initSPI() {
    spi_init(spi0, 1000 * 1000);  // 1 MHz
    gpio_set_function(16, GPIO_FUNC_SPI);  // MOSI
    gpio_set_function(18, GPIO_FUNC_SPI);  // SCK
    gpio_set_function(17, GPIO_FUNC_SPI);  // MISO (wenn nötig)
    gpio_pull_up(17);  // optional
}

void sendSPIStatus(uint8_t statusCode) {
    spi_write_blocking(spi0, &statusCode, 1);
    lastSPISignalTick = to_ms_since_boot(get_absolute_time());
}

uint8_t getSPIStatusCode(const char* phase) {
    if (strcmp(phase, "ROT") == 0) return CODE_RED;
    if (strcmp(phase, "ROT-GELB") == 0) return CODE_RED_YELLOW;
    if (strcmp(phase, "GRUEN") == 0) return CODE_GREEN;
    if (strcmp(phase, "GRUEN_BLINKEND") == 0) return CODE_GREEN_BLINK;
    if (strcmp(phase, "GELB") == 0) return CODE_YELLOW;
    if (strcmp(phase, "GELB_BLINKEND") == 0) return CODE_YELLOW_BLINK;
    return CODE_ERROR_DEFAULT;
}