// traffic_light_control.h
#ifndef TRAFFIC_LIGHT_CONTROL_H
#define TRAFFIC_LIGHT_CONTROL_H

#include <stdint.h>

#define CODE_RED             0b1110
#define CODE_RED_YELLOW      0b1101
#define CODE_GREEN           0b0010
#define CODE_GREEN_BLINK     0b0101
#define CODE_YELLOW          0b1000
#define CODE_YELLOW_BLINK    0b0001

void initOutputPin();                   // Konfiguriert den GPIO
void sendBitPattern(uint8_t code);      // Sendet 4-Bit-Muster auf GPIO
uint8_t getStatusCode(const char* phase);

#endif