#include <SoftwareSerial.h>
SoftwareSerial expandPin(11, 12);
#include "lib.h"
 


void setup() {
  // put your setup code here, to run once:
einit();
Serial.begin(9600);
delay(1000);
//epinMode(5,INPUT_PULLUP);
//epinMode(3,0);
 epinMode(13,1);
//int a = eanalogRead(3);
//Serial.println(a);
}

void loop() {
// if(edigitalRead(5)==0){
//  edigitalWrite(13,1);
// }else{
//  edigitalWrite(13,0);
// }
// delay(10);
edigitalWrite(13,1);
delay(1000);
edigitalWrite(13,0);
delay(1000);
//eArduino();
}
