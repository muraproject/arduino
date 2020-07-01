#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
#include <Servo.h>

Servo lift;
Servo grip;
Servo camYaw;

#define take      0
#define drop      1

#define liftTakePos 60      //posisi naik lift
#define liftDropPos 140     //posisi turun lift
#define gripTakePos 160     //posisi jepit
#define gripDropPos 60      //posisi lepas grip
#define camLeft     10    //posisi kamera menghadap ke kiri
#define camRight    170   //posisi kamera menghadap ke kanan
#define camForward  90    //posisi kamera menghadap ke depan

int gripState = 0;

String tampung;
String fromMaze;
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  mySerial.begin(9600);

  lift.attach(6);
  grip.attach(7);
  camYaw.attach(8);
  delay(500);
  actionDrop();
  camYaw.write(camForward);

  //Serial.print("cek");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (mySerial.available()) {
    char c = mySerial.read();
    if (c == 'x') {
      Serial.print("c");
    }

    if (c == 't') {
      //Serial.println("Mengambil");
      actionTake();
    }
    else if (c == 'd') {
      //Serial.println("Melepas");
      actionDrop();
    }
    else if (c == 's') {
      //Serial.println("Melepas");
      actionTrash();
    }

  }


  while (Serial.available()) {
    // get the new byte:
    tampung = Serial.readStringUntil('n');
  }
  if (tampung != "") {
        if (tampung == "m") {
          mySerial.print("R");
        }
        if (tampung == "k") {
          mySerial.print("Y");
        }
        if (tampung == "h") {
          mySerial.print("G");
        }
        if (tampung == "c") {
          mySerial.print("C");
        }
        if (tampung == "b") {
          mySerial.print("B");
        }

       //mySerial.print(tampung);

    tampung = "";
  }
}

void actionDrop() {
  lift.write(liftDropPos);
  delay(500);
  grip.write(gripDropPos);
}

void actionTrash() {
  lift.write(liftTakePos);
  
  
}

void actionTake() {
  grip.write(gripTakePos);
  delay(500);
  lift.write(liftTakePos);
}
