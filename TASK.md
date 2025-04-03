# Embedded Devices "Time-critical data-transmission" - Taskdescription

## Einführung
Diese Übung basiert auf den Grundlagen aus hardwarenaher Programmierung. Sie vertieft die Verwendung von Mikrokontrollern und die Weitergabe von Daten mittels Anbindung an Single-Board-Computern. 

## Ziele
Das Ziel der Übung ist das Verständnis und das Handling von gängigen Mikrokontroller-Toolchains zu vertiefen und komplexere Systeme zu entwerfen. Die Verwendung von Prioritäten bei Interrupts als auch die Recherche von real-time Umgebungen wird geschult.

## Voraussetzungen
* Grundverständnis von Mikrokontroller-Programmiernug und das Deployment mittels Toolchains
* Grundkenntnisse über die sichere Verkabelung von embedded Devices
* Verwendung von Interrupts
* Einsatz von Single-Board-Computern

## Detailierte Ausgabenbeschreibung
Basierend auf der Übung "Ampel mit LEDs" soll eine Ampelschaltung erweitert werden. Der aktuelle Status der Ampel muss an einen SPI-Bus zur weiteren Verwendung (z.B. Raspberry-Pi) übertragen werden. Bei einer fehlerhaften Übertragung oder einer fehlerhaften Datenleitung (keine Verbindung zum SPI Bus), muss der Fehlerzustand eingeschaltet werden (Gelb blinkend).

### Implementierung
Verwenden Sie zur Implementierung eine konsolen-basierte Deployment-Umgebung zur Programmierung und zum Flashen Ihrer Umsetzung (z.B. [stm32f4-template](https://github.com/mborko/stm32f4-template)). Die einzelnen Ampelstatus sollen kodiert an den SPI-Bus angelegt werden. Dabei sollen die entsprechenden Zeiten pro Bit genau eingehalten werden:

| Status        | Code      |
| :-----------: | :-------: |
| Rot           | 1-1-1-0   |
| Rot-Gelb      | 1-1-0-1   |
| Grün          | 0-0-1-0   |
| Grün blinkend | 0-1-0-1   |
| Gelb          | 1-0-0-0   |
| Gelb blinkend | 0-0-0-1   |

Sollte für mehr als 60 ms kein HIGH Signal am Bus beobachtet werden, muss die Ampelsteuerung entsprechend neu gestartet werden. Dies kann entweder mit einem Watchdog geschehen, oder mit hoch-priorisierten Interrupts. Nach dem Neustart ist zuerst der Fehlerzustand zu starten (Gelb blinkend). Nur nach erfolgreicher Verbindung zum SPI-Bus soll die Ampel gestartet werden.

## Bewertung
Gruppengrösse: 1-2 Person(en)
### GK Anforderungen **überwiegend erfüllt**
- [ ] Beantwortung der Fragestellungen
- [ ] Verwendung von konsolenbasiertem Deployment
### GK Anforderungen **zur Gänze erfüllt**
- [ ] Umsetzung der Ampelsteuerung und Ausgabe auf den SPI-Bus
- [ ] Ausführung und Dokumentation der Implementierung
- [ ] Ausgabe der Information des SPI-Busses mittels eines Logic-Analyzers
### EK Anforderungen **überwiegend erfüllt**
- [ ] Ausgabe der Information des SPI-Busses über einen Single-Board-Computer
### EK Anforderungen **zur Gänze erfüllt**
- [ ] Reset der Ampelsteuerung mittels Watchdog oder Interrupt-Implementierung

## Fragestellungen für das Protokoll
+ Was ist ein SPI-Bus und wie ist dieser aufgebaut?
+ Welche Vorteile ergeben sich bei der Verwendung eines Kommunikationsbusses?
+ Welche Möglichkeiten der Beschaltung sind beim SPI-Bus möglich und wie wirkt sich die Clock darauf aus?
+ Wie werden zeitkritische Anwendungen (real-time) eingeteilt?
+ Wie kommt ein Watchdog bei zeitkritischen Anwendungen zum Einsatz?
+ Wie kann man Interrupts priorisieren?
+ Was sind Real-Time Operating-Systems (RTOS) und wie kann man diese auf Mikrokontrollern einsetzen?

### Classroom Git-Repository
[Hier](https://github.com/500) finden Sie das Abgabe-Repository zum Entwickeln und Commiten Ihrer Lösung. Sollte der Server durch einen unerwarteten Umstand daran gehindert worden sein, die an ihn gesendete Anfrage zu erfüllen, muss der Link zu Beginn des Labors persönlich beantragt werden!

## Quellen
* [Ampelkunde](https://www.wien.gv.at/verkehr/ampeln/ampelkunde.html)
* [SPI for STM32f4](https://stm32f4-discovery.net/2014/04/library-05-spi-for-stm32f4xx/)
* [SPI for RaspberryPi](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md)
* [Logic Analyzer](https://www.saleae.com/de/downloads/)
* [Independent watchdog timer](https://stm32f4-discovery.net/2014/07/library-20-independent-watchdog-timer-on-stm32f4xx/)

---
**Version** *20210217v2*
