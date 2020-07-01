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
int state = 1;
String abc;
int cond = 2;

void setup() {

  USE_SERIAL.begin(9600);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  pinMode(2, 1);

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.println("[SETUP]");
    //USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("telkom2", "1234567890");
  delay(5000);

}

void loop() {
  // wait for WiFi connection
  
  Serial.print(cond);
 // Serial.print("|");
  abc = "";
  //String b="";

  while (1) {
    while (Serial.available()) {
      // char c =Serial.read();
      abc += Serial.readStringUntil('|');

    }
    if (abc.length() > 10) {
      break;

    }
  }
  while (1) {
    if ((WiFiMulti.run() == WL_CONNECTED)) {




      String b = "http://monitoring.domainpunyaku.com/mikro.php?";
      b += abc;

      HTTPClient http;

     // USE_SERIAL.print(abc);
      //USE_SERIAL.println("");
     // USE_SERIAL.println(b);
      // configure traged server and url
      //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
      http.begin(b); //HTTP  bahan1=1&bahan2=1&bahan3=1&campuran=1

      USE_SERIAL.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          if (payload.indexOf('1')) {
            digitalWrite(2, 1);
            //USE_SERIAL.println("ON");
            cond=1;
          } else {
            digitalWrite(2, 0);
           // USE_SERIAL.println("OFF");
            cond=0;
          }
          USE_SERIAL.println("succes");
        }
      } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        cond=2;
      }

      http.end();
      //Serial.flush();
      break;



    }

    delay(2000);
  }

}
