/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(D5, D6);
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

  USE_SERIAL.begin(9600);
  // USE_SERIAL.setDebugOutput(true);
  pzem.setAddress(0x42);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("monitoringpv", "12345678");

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    float volt = pzem.voltage();
    Serial.print("Voltage: ");
    Serial.print(volt);
    Serial.println("V");

    float cur = pzem.current();
    Serial.print("Current: ");
    Serial.print(cur);
    Serial.println("A");

    float powe = pzem.power();
    Serial.print("Power: ");
    Serial.print(powe);
    Serial.println("W");

    float ener = pzem.energy();
    Serial.print("Energy: ");
    Serial.print(ener, 3);
    Serial.println("kWh");

    float freq = pzem.frequency();
    Serial.print("Frequency: ");
    Serial.print(freq);
    Serial.println("Hz");

    float pf = pzem.pf();
    Serial.print("PF: ");
    Serial.println(pf);
    delay(1000);

    int watt = powe * 100.0;
    int volta = volt * 100.0;
    int ampere = cur * 100.0;
    int kwh = ener * 100.0;
    int hz  = freq * 100.0;
    int derajat = pf * 100.0;

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    String Url = "http://monitoringpv.com/php/dashboard.php?code=mikro&w=";
    Url += watt;
    Url += "&v=";
    Url += volta;
    Url += "&a=";
    Url += ampere;
    Url += "&k=";
    Url += kwh;
    Url += "&c=";
    Url += derajat;
    Url += "&f=";
    Url += hz;
    http.begin(Url); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(1000);
}
