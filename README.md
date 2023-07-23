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


## Developer notes:
My notes.
### UART to RP2040 with local echo: 
```sh
minicom -b 115200 -D /dev/ttyACM0
CTRL A + E  
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
| 25 | Current | Current=modelID, Capacity = compound date |
| 28 | Temp-F1713 | SBUS hold frames|
| 29 | Temp-F1713 | SBUS failsafe frames |
