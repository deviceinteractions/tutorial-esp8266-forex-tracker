# Tutorial for creating a Forex Rate tracker using ESP8266 and Arduino Uno
An ESP8266 + Arduino Uno Project that indicates the movement of a Forex Exchange Rate with LED indications. 

Created using the ESP8266 + Arduino Kit

## Overview
This project demonstrates an advanced implementation using the ESP8266/NodeMCU along with the Arduino Uno

The ESP8266 connects to the internet via WiFi and grabs the EUR/USD Forex Rates every 10 seconds.
Based on the movement of this value, it sends a UART message to the Arduino Uno (Rise / Fall / Unchanged)
The Arduino Uno receives this UART message and accordingly lights up an indication LED (Green / Red / Yellow)

## The ESP8266 side
The ESP8266 carries out the Internet-facing functions
1. It connects to a defined WiFi network
2. It makes an HTTP GET request to an online Forex Rates Server 
3. It receives the response from the Server
4. It filters the received data and stores only the required value
5. It compares this value with the value received during the previous request
6. Depending on the comparison with the previous value, it decides whether it is a RISE, FALL or CONST (unchanged)
7. It sends this as a Software Serial Message to the connected Arduino Uno for LED indication

## The Arduino Uno side
The Arduino Uno functions here as an indication controller, based on the Serial commands received
1. It defines the known messages (same as the ones defined on the ESP8266 side)
2. It opens a Software Serial bus and listens for messages
3. On receiving a message, it compares it with the know messages
4. On finding a match, it lights up the corresponding LED (Green for RISE, Red for FALL, Yellow for CONST/unchanged)

## Connections:
### ESP8266 to Arduino Uno


      (ESP) D1     ->     (Uno) 11
      
      (ESP) D2     ->     (Uno) 10
      
      (ESP) Vin    ->     (Uno) 5V

      (ESP) Gnd    ->     (Uno) Gnd
      
Make sure to connect only the ESP8266/NodeMCU to a PC via USB for Serial messages as well as power supply

### Arduino Uno to LEDs
pin 5 -> Red LED (via 330 Ohm LED)
pin 6 -> Yellow LED (via 330 Ohm LED)
pin 7 -> Green LED (via 330 Ohm LED)

Make sure to use a 330Ohm resistor from the Arduino pin to each LED.
Make sure to connect the other end of the LEDs together and to the Arduino Ground

Arduino pin 5 ---^^^330Ohm^^^---->I(RED LED)------Arduino Gnd

Arduino pin 6 ---^^^330Ohm^^^---->I(YELLOW LED)---Arduino Gnd

Arduino pin 7 ---^^^330Ohm^^^---->I(GREEN LED)----Arduino Gnd

### Refer to the Connection Diagram Image in the repository
