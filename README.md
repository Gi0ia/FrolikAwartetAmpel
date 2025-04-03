# Embedded Devices "Time-critical data-transmission"
*von Awart & Frolik*
2025-04-03

## Aufgabenstellung  
Die detaillierte [Aufgabenstellung](TASK.md) beschreibt die notwendigen Schritte zur Realisierung.

## Recherche  
- Grundlagen zu FreeRTOS auf dem RP2040 (z. B. Raspberry Pi Pico)  
- SPI-Kommunikation zwischen Mikrocontroller und Single-Board-Computer  
- Zeitkritische Steuerung mit Interrupts und Watchdog  
- Einsatz eines Logic Analyzers zur Busüberwachung  

## Implementierung  
- FreeRTOS-basierte Ampelsteuerung mit separaten Tasks für jede Phase  
- Fehlerbehandlung via Prioritäts-Task (gelb blinkend bei Fehler)  
- Modularisierung durch Header (`traffic_light_control.h`)  
- Vorbereitung für SPI-Übertragung von Statuscodes (mapping & send-Funktion)  
- Konsolenbasiertes Debugging über USB (stdio)  

## Quellen  
- FreeRTOS Demo für Raspberry Pi Pico  
- RP2040 Datasheet (SPI, GPIO, Timer)  
- Logic Analyzer Dokumentation (z. B. Saleae, PulseView)  
- Ampellogik nach deutscher StVO  
