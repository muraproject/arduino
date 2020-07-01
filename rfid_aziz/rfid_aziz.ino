#include <SPI.h>
#include <RFID.h>
#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);
int state = 1;

void setup() {
  Serial.begin(9600);
  pinMode(3, 1);
  pinMode(4, 1);
  SPI.begin();
  rfid.init();
//  while (1) {
//    digitalWrite(4, 1);
//    delay(1000);
//    digitalWrite(4, 0);
//    delay(1000);
//  }
}

void loop() {

  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      Serial.print(rfid.serNum[0]);
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.println(rfid.serNum[3]);
    }
    rfid.halt();
    if (rfid.serNum[0] == 214 && rfid.serNum[1] ==  169  && rfid.serNum[2] == 155 && rfid.serNum[3] == 3 ) {
      if (state == 1) {
        digitalWrite(4, 1);
        digitalWrite(3, 1);
        state = 0;
        delay(1000);
        digitalWrite(3, 0);
        delay(9000);
       
      } else {
        state = 1;
        digitalWrite(3, 1);
        delay(500);
        digitalWrite(3, 0);
        delay(500);
        digitalWrite(4, 0);
        delay(3000);
        
      }

    } else {
      digitalWrite(3, 1);
      delay(500);
      digitalWrite(3, 0);
      delay(500);
      digitalWrite(3, 1);
      delay(500);
      digitalWrite(3, 0);
      delay(500);
      digitalWrite(3, 1);
      delay(500);
      digitalWrite(3, 0);
      delay(500);
    }
  }
}
