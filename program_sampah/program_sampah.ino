#include <Servo.h>
Servo s1;
Servo s2;
Servo s3;
int count = 0;
int state = 0;
char tampung1;
String a = "";
const int pingPin1 = 8;
const int pingPin2 = 7;
String tampung = "";
unsigned long millis1 = 0, millis2 = 0;
void setup() {
  // put your setup code here, to run once:
  s1.attach(9);
  s2.attach(10);
  s3.attach(6);
  pinMode(2, 2);
  pinMode(A0, 2);
  pinMode(A1, 2);
  pinMode(13, 1);
  s1.write(90);
  s2.write(0);
  Serial.begin(9600);
  // Serial.println("start");

}

void loop() {
  // put your main code here, to run repeatedly:
  while (count == 0) {
    millis1 = millis();
    if (millis1 - millis2 > 5000) {
      // Serial.println("data cek");
      long duration, duration2, cm, cm2;
      // for (int i = 0; i < 100; i++) {
      //  delay(50);

      pinMode(pingPin1, OUTPUT);
      digitalWrite(pingPin1, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin1, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin1, LOW);
      pinMode(pingPin1, INPUT);
      duration = pulseIn(pingPin1, HIGH);
      cm = microsecondsToCentimeters(duration);

      delay(50);

      pinMode(pingPin2, OUTPUT);
      digitalWrite(pingPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin2, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin2, LOW);
      pinMode(pingPin2, INPUT);
      duration2 = pulseIn(pingPin2, HIGH);

      cm2 =  microsecondsToCentimeters(duration2);
      //  }

      //    cm = cm/100.0 ;
      //    cm2 = cm2/100.0 ;
      //Serial.println("cm : ");
      Serial.println(cm);
      Serial.println(cm2);
      if (cm < 5) {
        //Serial.print("L");
        //kedip();
      }
      if (cm2 < 5) {
        //Serial.print("N");
        //kedip();
      }
      delay(1000);

      millis2 = millis1;

    }

    if (!digitalRead(A1)) {
      delay(1000);
      if (!digitalRead(A1)) {
        s2.write(90);
      }
      if (a != "b" && digitalRead(A0)) {
        a = "b";
        // Serial.print("b");
      }
    } else
    {
      delay(1000);
      if (digitalRead(A1)) {
        s2.write(0);
      }
      if (a != "t" && digitalRead(A0)) {
        a = "t";
        // Serial.print("t");
      }
    }

    if (!digitalRead(2) && !digitalRead(A1)) {
      s1.write(0);
      // Serial.print("l");
      state = 1;
      delay(3000);
      s1.write(90);
      delay(1000);
      Serial.print("p");
      count = 1;
    }

    else if (!digitalRead(A0) && !digitalRead(A1)) {
      delay(1000);
      //    if (digitalRead(2)) {
      s1.write(180);
      //Serial.print("n");
      state = 1;
      delay(3000);
      s1.write(90);
      delay(1000);
      Serial.print("p");
      count = 1;
    }
    else {

    }
  }
  while (count == 1) {

    //Serial.print("p");
    //
    //    while (!Serial.available()) {
    //
    //    }

    while (Serial.available()) {
      // get the new byte:
      tampung = Serial.readStringUntil('n');
    }
    if (tampung != "") {
      if (tampung == "b") {
        
        s3.write(0);
        delay(3000);
        s3.write(90);
        delay(1000);
      }
      if (tampung == "t") {
        s3.write(180);
        delay(3000);
        s3.write(90);
        delay(1000);
      }


      //mySerial.print(tampung);
      count = 0;

      tampung = "";
    }
  }



}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void kedip() {
  for (int u = 0; u < 10; u++) {
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
    delay(500);
  }
}
void kedip1() {
  for (int u = 0; u < 5; u++) {
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
    delay(500);
  }
}
