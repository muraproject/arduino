/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
//#include <PZEM004T.h>

#include <ESP8266HTTPClient.h>
#include "url.h"

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

int kecepatan = 160;

float arus, tegangan, daya, kwh;

void ap_mode();
int _mode,count_=0;
int econd,kondisi,_onstate,mOnstate;
int compare;
int _restart;
int cond_;
int counter=0;

// Replace with your network credentials
const char* ssid     = "Deteksi Masker";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String esid;
String epass;

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
#include "AP_.h"
void setup() {
  
  EEPROM.begin(1024);
  pinMode(D5,1);
  pinMode(D6,1);
  pinMode(D7,1);
  pinMode(D1,INPUT_PULLUP);
  pinMode(D2,INPUT_PULLUP);
  pinMode(2,1);


   
    delay(1000);
     Serial.begin(9600);
 
  WiFi.softAP(ssid, password);
  delay(2000);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
  delay(2000);
    
//  }

  
}

void loop(){


  ap_mode();

    
 
}
