/* A sketch that listens for Software-Serial Messages and accordingly lights up LEDs

   This is the Arduino Uno part of the ESP8266+Arduino Forex Rate Tracker project
   *Flash this to the Arduino Uno
   
   Ranjit Bhinge,
   Device Interactions

   www.device-interactions.com
   www.github.com/deviceinteractions
   
   http://blog.device-interactions.com
   http://shop.device-interactions.com

   This example listens for known commands on a Software Serial UART bus
   It has 3 LEDs connected 
   Pin 5 -> Red LED
   Pin 6 -> Yellow LED
   Pin 7 -> Green LED

   It is designed to flash an LED to indicate the behaviour of a parameter over time
   RISE -> Green LED | FALL -> Red LED | NO CHANGE -> Yellow LED
   
   It has pre-defined coded messages that it receives from the ESP8266
   '>' - RISE
   '|' - CONST / NO CHANGE
   '<' - FALL
   
   
*/

#include <SoftwareSerial.h>

#define RED_PIN 5
#define YELLOW_PIN 6
#define GREEN_PIN 7
#define RISE'>'
#define CONST '|'
#define FALL '<'
// create a Software Serial instance for communications with the ESP8266
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  
  mySerial.begin(9600); // init Software Serial
  Serial.begin(9600);   // init normal USB/Hardware Serial

  // initiate all the LED pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void loop() {
  // Listen for software Serial messages
  if(mySerial.available()>0){
    char in = mySerial.read();

    // check whether the message received is a known one, respond accordingly
    if(in == RISE){
      Serial.println("RISE");
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
    }
    else if(in == CONST){
      Serial.println("CONST");
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(RED_PIN, LOW);
    }
    else if(in == FALL){
      Serial.println("FALL");
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
    }
    
    
    Serial.println(in);
    Serial.println();
  }
    delay(2000);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(RED_PIN, LOW);

}
