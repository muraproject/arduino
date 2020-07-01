/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
#include <ESP8266WiFi.h> //memasukan library ESP8266
#include "DHT.h" //memasukan library dht11

//mendefinisikan sensor yang dipakai (DHT11, DHT21, dan DHT22)
#define DHTTYPE DHT11   //tipe yang dipilih DHT 11
#define triggerPin  D3
#define echoPin     D4

const int DHTPin = 5;
//inisialisasi library DHTpin
DHT dht(DHTPin, DHTTYPE);

void setup() {

  USE_SERIAL.begin(115200);
  dht.begin();
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("telkom2", "1234567890");

}

void loop() {
  // wait for WiFi connection


  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
//    int h = dht.readHumidity();
//    // Baca suhu sebagai Celsius (default)
//    float tu = dht.readTemperature();
    int t = t;

    long duration, jarak;
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    jarak = (duration / 2) / 29.1;
    Serial.print(jarak);
    Serial.println(" cm");
    t=jarak;
    t=30-t;
    if(t>30){
      t=30;
    }
    if (t<0){
      t=0;
    }
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    String url = "http://us-central1-muraproject-b57a2.cloudfunctions.net/project/post/?id=suhuku&suhu=";
    url += t;

    http.begin(url); //HTTP

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

  delay(3000);
}
