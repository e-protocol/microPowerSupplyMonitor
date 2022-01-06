********************************************************************************
<p style='font-size:18px'>Micro Power Supply 5v for Arduino/ESP32 projects</p><br/>
<p float="left">
<img src = "Project Photo/microPowerSupplyMonitor_pic1.jpg" width = "200" height = "400" />
<img src = "Project Photo/microPowerSupplyMonitor_pic2.jpg" width = "400" height = "400" />
<img src = "Project Photo/microPowerSupplyMonitor Schematic.jpg" width = "400" height = "200" />
</p>
Created by E-Protocol
https://github.com/e-protocol
<br/>
Specs: Arduino
Description: When you need an extra 5v power source, here comes a micro power supply. Very useful device
to power Arduino or ESP32 breadboard projects. Also can power up whatever you need via usb or even charge
your phone. Sometimes it needs to know the device's power demand, so it has a display to view such 
information.
<br/>
********************************************************************************
<br/>
Arduino part
<br/>
********************************************************************************

Component list:
1) Arduino Nano
2) DC-DC converter 0.9-5V Input 5V Output or similar
3) TP4056 charge module for 18650 battery
4) 18650 battery 3000 mA
5) battery holder
6) double USB Female port
7) x2 leds 3v
8) switcher
9) 0.25mm wires
10) plastic electrical box
11) some Solder and flux
12) some hot glue
13) x4 bolts
14) INA_219 module
15) OLED Display 126x64

NOTE: 
This project requires extra libraries: Adafruit_INA219, U8glib. Which could be 
downloaded by Arduino IDE library manager.
