# Embedded Devices "Time-critical data-transmission"
*von Awart & Frolik*
2025-04-03

## Aufgabenstellung  
Die detaillierte [Aufgabenstellung](TASK.md) beschreibt die notwendigen Schritte zur Realisierung.


## Status-Codierung (4 Bit)

| Phase           | Code (binär) |
|-----------------|--------------|
| Rot             | `1110`       |
| Rot-Gelb        | `1101`       |
| Grün            | `0010`       |
| Grün blinkend   | `0101`       |
| Gelb            | `1000`       |
| Gelb blinkend   | `0001`       |

### Gleichrangige Ampel-Tasks (Priorität 1)
- Alle Tasks (Grün, Grün blinkend, Gelb, Rot, Rot-Gelb) haben die gleiche Priorität.
- Das bedeutet: FreeRTOS schedult sie nur, wenn sie explizit aktiviert wurden (was du eh per xTaskNotifyGive() machst).
- Solange kein Fehler vorliegt, laufen die Tasks sequentiell

## Recherche  
- Grundlagen zu FreeRTOS auf dem RP2040 (z. B. Raspberry Pi Pico)  
- GPIO-basierte Bitübertragung als einfache SPI-Alternative  
- Zeitkritische Steuerung mit Interrupts und Task-Prioritäten  
- Einsatz eines Logic Analyzers zur Protokollanalyse und Fehlerdiagnose  
- Mapping von Ampelphasen in binäre Statuscodes

## Implementierung  
- **FreeRTOS-Taskstruktur:**  
  - Separate Tasks für Rot, Rot-Gelb, Grün, Grün blinkend, Gelb  
  - Fehlerzustand (Gelb blinkend) mit höchster Priorität  

- **Bitweise GPIO-Übertragung:**  
  - 4-Bit-Codes je Ampelphase werden seriell über einen definierten GPIO (z. B. GPIO15) ausgegeben  
  - Timing: **5 ms pro Bit** → Gesamtübertragung: **20 ms**  
  - Überwachung mit Logic Analyzer möglich  

- **Fehlerbehandlung:**  
  - Fehler-Task (blinkend gelb) wird aktiv, wenn `errorFlag == true`  
  - Zentrale Statusübertragung via `sendBitPattern()`  

- **Modularisierung:**  
  - Header-Datei `traffic_light_control.h` mit Statuscodes, GPIO-Funktionen und Mapping  
  - Flexibel erweiterbar für SPI, Watchdog oder komplexere Überwachung  

## Fehlerbehandlung über GPIO-Interrupt (ISR) - EK

- Über einen externen GPIO (z. B. GPIO16) kann ein Fehlerzustand ausgelöst werden.
- Die ISR setzt `errorFlag = true` und aktiviert sofort den `ErrorTask`.
- Gelb blinkt mit 500 ms Intervallen.
- Während `errorFlag == true` werden alle anderen Tasks blockiert.

### ISR-Ablauf (Interrupt Service Routine):

```c++
void gpio_error_isr(uint gpio, uint32_t events) {
    errorFlag = true;
    vTaskNotifyGiveFromISR(xErrorTask, ...);
}
```

## Quellen  
- FreeRTOS Demo für Raspberry Pi Pico  
- RP2040 Datasheet (SPI, GPIO, Timer)  
- Logic Analyzer Dokumentation (z. B. Saleae, PulseView)  
- Ampellogik nach StVO  
- https://chatgpt.com/share/67ee54d9-3010-8011-a7e1-77ee257ae5e2
