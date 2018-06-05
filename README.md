# AutomaticFlowerpot

## Overview:
The project of automatic flowerpot, it checks humidity level and waters the flower if necessary.

## Tools:

Tools required to build this project

* STM32F4-Discovery board,
* Moisture sensor,
* Electric Liquid Pump 12V,
* Relay Module 5V,
* Power supply 12V to power the pump.
* Power bank to power the board.
* RGB diode module
* System Workbench for STM32 IDE

Optional:
* Water Tank,
* Plastic pipes,
* Flowerpot
## How to compile

Download the project and compile it using System Workbench for STM32

## How to run:


Moisture Sensor | Stm32
------------ | -------------
AO | GPIOA1
VCC | 3V
GND | GND

Relay Module | Stm32
------------ | -------------
IN1 | GPIOPA3
VCC | 5V 
GND | GND 

Cable from 12V power source has to go through middle and normally open contact.

RGB diode module | Stm32
------------ | -------------
Red/Blue/Greeb Color Contact | GPIOPD3
GND | GND

Humidity level at which pump is turned on can be set via IDE, before compiling.

## Future improvements:
* Step down voltage converter to power the board with the same power supply as the pump.
* Internet connection to notify user when water tank is empty.

## Attributions:

## License: MIT

## Credits:

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology. Supervisor: Tomasz Mańkowski
