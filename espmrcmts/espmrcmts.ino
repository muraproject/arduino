/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D6, D5);


ESP8266WiFiMulti WiFiMulti;
int count;

void setup() {

  USE_SERIAL.begin(115200);
  pinMode(2,1);
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
  WiFiMulti.addAP("telkom2", "12345678");

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;
    String mUrl="";

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    if(count == 0){
    mUrl = "http://jatayu.io/api/data/4a779328-02d1-11ea-9897-17db0790be09/v1?value=";
    int magnetku = get_magnet().toInt();
    mUrl += magnetku;
    digitalWrite(2,1);
    }

    if(count == 1){
    mUrl = "http://jatayu.io/api/data/4a779328-02d1-11ea-9897-17db0790be09/v2?value=";
    String value1 = get_luminance();
    int lum = value1.toInt();
    mUrl += lum;
    }

    if(count == 2){
    mUrl = "http://jatayu.io/api/data/4a779328-02d1-11ea-9897-17db0790be09/v3?value=";
    int apiku = get_api().toInt();
    mUrl += apiku;
    digitalWrite(2,0);
    }
    count++;
    if(count==3){
      count=0;
    }
    
    http.begin(mUrl); //HTTP
    Serial.println(mUrl);

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

  delay(100);
}



String get_luminance() {
  String tampung = "";
  mySerial.print('l');
  while (!mySerial.available()) {
    delay(1);
  }
  while (mySerial.available()) {
    // get the new byte:
    tampung = mySerial.readStringUntil('\n');
  }
  if (tampung != "") {

  }

  //mySerial.print(tampung);

  return tampung;
}


String get_magnet() {
  String tampung = "";
  mySerial.print('m');
  while (!mySerial.available()) {
    delay(1);
  }
  while (mySerial.available()) {
    // get the new byte:
    tampung = mySerial.readStringUntil('\n');
  }
  if (tampung != "") {

  }

  //mySerial.print(tampung);

  return tampung;
}

String get_api() {
  String tampung = "";
  mySerial.print('a');
  while (!mySerial.available()) {
    delay(1);
  }
  while (mySerial.available()) {
    // get the new byte:
    tampung = mySerial.readStringUntil('\n');
  }
  if (tampung != "") {

  }

  //mySerial.print(tampung);

  return tampung;
}


