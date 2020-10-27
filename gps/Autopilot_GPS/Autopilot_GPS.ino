#include <EasyTransfer.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>

TinyGPSPlus gps;
SoftwareSerial g(2, 3);

EasyTransfer tx; 

struct SEND_DATA_STRUCTURE{
  float latitude;
  float longitude;
};

SEND_DATA_STRUCTURE loc;

void setup() {
  MCUCR = _BV (BODS) | _BV (BODSE);
  MCUCR = _BV (BODS); 
  Serial.begin(38400);
  g.begin(9600);
  tx.begin(details(loc), &Serial);
}

void loop() {
  while (g.available() > 0) {
    gps.encode(g.read());
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
  if (gps.location.isValid()){
    loc.latitude = gps.location.lat();
    loc.longitude = gps.location.lng();
    digitalWrite(13, HIGH);
    if (gps.location.age() < 2000) {
      //if (!tx.sendData()){
      //  
      //}
      tx.sendData();
    }
  }
  delay(50);
  digitalWrite(13, LOW);
}

