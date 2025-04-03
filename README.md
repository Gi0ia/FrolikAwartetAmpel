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

## Quellen  
- FreeRTOS Demo für Raspberry Pi Pico  
- RP2040 Datasheet (SPI, GPIO, Timer)  
- Logic Analyzer Dokumentation (z. B. Saleae, PulseView)  
- Ampellogik nach StVO  
