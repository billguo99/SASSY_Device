# SASSY_Device
Arduino code for implementing the RFID Card Reader Device on a ESP32 for the Student Attendance and Submission System.

## Individual Component Testing
In this folder, Arduino code used for testing individual components of the RFID Card Reader Device can be found. The individual components tested with the ESP32 are LED, OLED display, Push Button, RDM6300 RFID Reader. In addition, the following services on the ESP32 were tested: Wi-Fi, JSON HTTP POST and GET requests, NTP server. These services were used to integrate with the SASSY_API developed by Keegan Crankshaw.

## RFID Card Reader Device
In this folder, 3 prototypes of the RFID Card Reader device code can be found.
Prototype 1 - Logical flow of system design is implemented without any communication with SASSY_API.
Prototype 2 - GET Request integration with SASSY_API successfully implemented for RFID Card Reader device on Breadboard.
Prototype 3 - Full integration with SASSY_API successfully implemented for RFID Card Reader device on Veroboard.
