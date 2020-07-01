#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(D5,D6);// D6, D5, (RX,TX) connect to TX,RX of PZEM
//IPAddress ip(192, 168, 1, 1);
float arus, tegangan, daya, kwh;

void setup() {
  Serial.begin(9600);
  pzem.setAddress(0x42);
}

void loop() {
  tegangan = pzem.voltage();
  if (tegangan < 0.0) tegangan = 0.0;
  Serial.print(tegangan); Serial.print("V; ");

  arus = pzem.current();
  if (arus >= 0.0) {
    Serial.print(arus);
    Serial.print("A; ");
  }

  daya = pzem.power();
  if (daya >= 0.0) {
    Serial.print(daya);
    Serial.print("W; ");
  }

  kwh = pzem.energy();
  if (kwh >= 0.0) {
    Serial.print(kwh);
    Serial.print("Wh; ");
  }
  Serial.println();
  delay(1000);
}
