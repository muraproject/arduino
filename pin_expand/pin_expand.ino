#include <SoftwareSerial.h>
//SoftwareSerial expandPin(10, 11);
#include "lib.h"
String ch;


void setup() {
  // put your setup code here, to run once:
  einit();
  Serial.begin(9600);
  //delay(1000);
  //epinMode(5,INPUT_PULLUP);
  //epinMode(3,0);
  //epinMode(13,1);
  //int a = eanalogRead(3);
  //Serial.println(a);
}

void loop() {
while (expandPin.available()) {
    ch = expandPin.readStringUntil('\n');

  }

if (ch != "") {
    Serial.println(ch);
    //expandPin.print(ch);
    if (ch.startsWith("p")) { //pin mode
      setPin(ch);
    }

    if (ch.startsWith("d")) { //digitalread
      digitread(ch);
    }
    if (ch.startsWith("w")) { //digitalwrite
      digitwrite(ch);
    }
    if (ch.startsWith("r")) { //analogread
      analogread(ch);
    }

    if (ch.startsWith("a")) { //analogwrite

    }
    ch="";
  }

}
