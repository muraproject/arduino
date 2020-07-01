/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
//#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include "DHT.h"  

#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 
#define USE_SERIAL Serial

//ESP8266WiFiMulti WiFiMulti;

void ap_mode();


// Replace with your network credentials
const char* ssid     = "nodemcu";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign output variables to GPIO pins

int h,t;
#include "AP_.h"
void setup() {
 
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  server.begin();
  dht.begin();
  Serial.begin(9600);
  delay(2000);

  
}

void loop(){
            
//    Serial.print("Current humidity = ");
//    Serial.print(h);
//    Serial.print("%  ");
//    Serial.print("temperature = ");
//    Serial.print(t); 
//    Serial.println("C  ");
  //delay(800);
  ap_mode();
}
