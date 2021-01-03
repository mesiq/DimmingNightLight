A better sensor dimming LED night light with Arduino
===

Night light includes following features:
- ignoring motion detection based on light in the room
- on and off delay
- on duration
- separate fading smoothness for fade on and fade off
- automatic off after turning on regular light

You will need following parts:
- 5V power supply (if you use 12V PS, I recommend to use step down dc-dc converter to 3.3V to power LED)
- Arduino (I've used Arduino Nano)
- LED (I've used 0.5W warm white, 0.15A)
- PIR sensor
- Photoresistor
- 2x 10kOhm Resistor
- n-channel MOSFET transistor (I've used 2N700 which has limit 0.2A)
- Resistor/step down DC-DC converter in case you don't match your voltage definition to power your LED (in my case 10Ohm)


How to calculate Resistor value for your LED
---
You need to know specification of your LED:
1. V<sub>LED</sub>
2. I<sub>LED</sub> - maximum forward current
3. V<sub>Source</sub> - imput voltage

R = (V<sub>Source</sub> - V<sub>LED</sub>)/I<sub>LED</sub>

   In my case: R = (5-3.3)/0.15 =~ 11 Ohm so 10 Ohm resistor will be just fine.
   
_Note: Using Resistor instead of step-down converter is not efficient when using high voltage power supply - e.g. 12 V as resistor will consume power and transform it to heat._

Circuit diagram
---
Following diagram uses 10 Ohm LED Resistor to drop down voltage from 5V to 3.3V for LED which has forward current 150mA.
![alt text](https://github.com/mesiq/DimmingNightLight/blob/main/circuit.svg "Circuit diagram with 5V DC power supply")
