/* An HTTP Client that grabs Forex EUR/USD Rates from the internet

   This is the ESP8266 part of the ESP8266+Arduino Forex Rate Tracker project
   *Flash this to the ESP8266
   
   Ranjit Bhinge,
   Device Interactions

   www.device-interactions.com
   www.github.com/deviceinteractions
   
   http://blog.device-interactions.com
   http://shop.device-interactions.com

   This example sets up an ESP8266 as an HTTP Client
   The Client connects to a WiFi network and sends a GET request every 10 seconds
   to an online Forex Rates resource
   
   The resource responds with the detailed exchange rates data
   The code grabs the necessary parts of the response, cmpares it with
   the previously received response and decides wheteher it rose, fell or didn't change

   It accordingly sends a Serial message to the Arduino
   The Arduino accordingly lights up an LED as an indicator
   RISE -> Green LED | FALL -> Red LED | NO CHANGE -> Yellow LED

   
*/


#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>

// WiFi network definitions
#define WIFI_SSID "YOUR-WiFi-NAME"          // Change this to your Network's name
#define WIFI_PASS "YOUR-WiFi-PASSWORD"      // Change this to your Network's password

// Software Serial command definitions
#define RISE '>'
#define FALL '<'
#define CONST '|'

// online resource web address
String http_addr = "http://webrates.truefx.com/rates/connect.html?f=html";

// init global variables
String url = "";
String body = "";
int exchange = -1;
int prevExchange = -1;

// instantiate Software Serial for communication with the Arduino
SoftwareSerial SSerial(5, 4, false, 256); //RX, TX, invert, buffer length

void setup() {
    SSerial.begin(9600);    // init Software Serial
    Serial.begin(9600);     // init normal USB/Hardware Serial

    // init onboard LED (flashes while doing a HTTP call
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH); // switch it OFF. (HIGH means OFF for the ESP8266)
    
    Serial.println();
    Serial.println("Clearing Serial buffer...");
    Serial.flush();

    // connect to the WiFi network
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("\nConnecting to the Server's Access Point");  
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(D4, LOW);
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    digitalWrite(D4, HIGH);
}

void loop() {
  // if the WiFi connection is lost, reconnect
    if(WiFi.status() != WL_CONNECTED){
      while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(D4, LOW);
        delay(500);
        Serial.print(".");
      }
      
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(WIFI_SSID);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println();
      digitalWrite(D4, HIGH);
    }
    
      // initiate the HTTP connection
      HTTPClient http;
      url = http_addr;
      Serial.print("[HTTP] starting...\n");
      // configure traged server and url
      Serial.println(url);
      http.begin(url); //HTTP

      Serial.print("Sending message - ");
      Serial.println(body);
      // start connection and send HTTP header
      int httpCode = http.GET();
      if(httpCode > 0) {
          digitalWrite(D4, LOW);  // Blue flash for connection indication
          
          // If the request is valid
          if(httpCode == HTTP_CODE_OK) {
            Serial.print("Received Response\n");
            String payload = http.getString();

            // extracting only the most inportant part of the response
            payload = payload.substring(66, 69);
            Serial.print("EUR / USD Exchange Rate is x.xx");
            
            exchange = payload.toInt();
            Serial.println(exchange);

            // send the corresponding message based on the change in Forex value
            if(exchange>prevExchange){
              sendMessage(RISE);
              Serial.println("It has RISEN!");
            }
            else if(exchange<prevExchange){
              sendMessage(FALL);
              Serial.println("It has FALLEN!");
            }
            else if(exchange==prevExchange){
              sendMessage(CONST);
              Serial.println("It has NOT CHANGED");
            }
            Serial.println();
            Serial.println();
            Serial.flush();
            prevExchange = exchange;
            digitalWrite(D4, HIGH);
          }
      } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      // HTTP connection terminated
   
    delay(10000); 
    // wait for 10 seconds. Don't want the online resource to flag us as spammers
}

// function to send the Software Serial message to the Arduino
void sendMessage(char message){
  SSerial.print(message);
  delay(50);
  Serial.print(message);
  delay(50);
}

