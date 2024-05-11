# openXsensor (oXs) on RP2040 board
## For rc protocols : expressLRS / FRSKY (sport + Fbus) / HOTT / JETI Ex / JETI EXBUS/ MPX / FLYSKY / Futaba (SBUS2) / Spektrum (SRXL2) 

Modified project for personal use, using of Futaba system and GPS NEO-6M, MS5611 sensors. For the basic setup of the device, follow the instructions at https://github.com/mstrens/oXs_on_RP2040, only the differences compared to the original branch are described here.

## Difference between original branch
- LED behavior on RP2040 is same as on original Futaba sensors (flashing off, failsafe signaled with red color)
- added identification of modified version
- MS5611 sends the measured temperature via field TEMP1
- added support for GPS-NMEA (parsing library: https://github.com/mikalhart/TinyGPS )
- added date transfer support for Futaba transmitters that don't support date, transfer value = day << 8 | month << 4 | 2023-year. It is transmitted as a current sensor. Reconstructable with processLogs.py script.


## ChangeNote
### 1.0.0
- LED behavior on RP2040 is same as on original Futaba sensors (flashing off, failsafe signaled with red color)
- added identification of modified version
- MS5611 sends the measured temperature via field TEMP1
- added support for GPS-NMEA (parsing library: https://github.com/mikalhart/TinyGPS )
- added date transfer support for Futaba transmitters that don't support date, transfer value = day << 8 | month << 4 | 2023-year. It is transmitted as a current sensor. Reconstructable with processLogs.py script.

### 1.1.0
- modelID can be set without recompilation

### 1.2.0
- MAX6675 support added, default pin is 8,9,10

### 1.2.1
- MAX6675 - temp is sent only if data is available

### 1.2.2
- MS5611 - partial elimination of deviation caused by self-heating (for temperature only)
- GPS - correction of sending first valid data 

### 1.2.3
- add SD card logger (SD card library: https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico)
- switched SPI pins for SD card and MAX6675
- status LED indicate SD card error with orange light 
- convert script works on multiple csv files in folder

## Developer notes:
My notes.
### UART to RP2040 with local echo: 
```sh
minicom -b 115200 -D /dev/ttyACM0
CTRL A + E  - echo 
CTRL + J    - new line (enter)
```

### Buttonless flashing to RP2040
```sh
./picotool load -f oXs.uf2
```

### Futaba Slots:
| Slot | Sensor | Meaning |
| ------ | ------ | ------ |
|  0 (const)  | Receiver | - |
| 8 | GPS | - |
| 16 | Vario-F1672 | - |
| 21 | Temperature | from MS5611 |
| 22 | Temperature | from MAX6675 |
| 25 | Current | Current=modelID, Capacity = compound date |
| 28 | Temp-F1713 | SBUS hold frames|
| 29 | Temp-F1713 | SBUS failsafe frames |


### RP2040 zero wiring :
| PIN RP2040| PIN DEVICE |
| ------ | ------ |
|GP 0  | SD CARD - RX |
|GP 1  | SD CARD - CSN  |
|GP 2  | SD CARD - SCK |
|GP 3  | SD CARD - TX |
|GP 4  | SBUS - TELEMETRY|
|GP 5  | SBUS - PRIMARY|
|GP 8  | MAX6675 - RX |
|GP 9  | MAX6675 - CSN |
|GP 10 | MAX6675 - SCK | 
|GP 26 | MS5611 - SDA | 
|GP 27 | MS5611 - SCL |
|GP 28 | GPS - RX |
|GP 29 | GPS - TX |

