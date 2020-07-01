/**
   StreamHTTPClient.ino

    Created on: 24.05.2015

*/


#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
SoftwareSerial serialku(D2,D3);
ESP8266WiFiMulti WiFiMulti;
int state = 0;
String mUrl,mData;
int a;
void setup() {

  USE_SERIAL.begin(9600);
  serialku.begin(9600);
  pinMode(D6,1);
  pinMode(2,1);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  digitalWrite(D6,0);

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("OMYAS", "ojoberisik");

}

void loop() {

  if(state==0){
    digitalWrite(D6,1);
    digitalWrite(2,0);
  if(serialku.available()){
    
    while(serialku.available()){
      char c = serialku.read();
      mData +=c;
    }

    if(mData.length()>10){
      digitalWrite(D6,0);
      mData = mData.substring(mData.indexOf("{")+1,mData.indexOf("}")-1);
      
      Serial.println(mData);
      state=1;
     
    }else{
      digitalWrite(D6,0);
      delay(100);
    }
    
  }
  //delay(1);
  //a++;
   // if(a>1000){
     // Serial.println("wait");
   // }
  }
 
  // wait for WiFi connection
  if(state==1){
  digitalWrite(2,1);
  mUrl = "http://muraproject.com/inas/fromesp.php?";
  //mData= "vr=234&vs=238&vt=234&ar=234&as=234&at=234&t=234";
  String final_url;
  final_url +=mUrl;
  final_url +=mData;
  
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");

    // configure server and url
    http.begin(final_url);
    //http.begin("192.168.1.12", 80, "/test.html");

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get lenght of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();

        // create buffer for read
        uint8_t buff[128] = { 0 };

        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();

          if (size) {
            // read up to 128 byte
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            USE_SERIAL.write(buff, c);

            if (len > 0) {
              len -= c;
            }
          }
          delay(1);
        }

        USE_SERIAL.println();
        USE_SERIAL.print("[HTTP] connection closed or file end.\n");
        state=0;
        mData = "";

      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  delay(5000);
  }

  //delay(10000);
}

