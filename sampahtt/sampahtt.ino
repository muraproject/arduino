#include <Servo.h>
Servo s1;
Servo s2;
Servo s3;
Servo s4;
Servo s5;
Servo s6;
Servo s7;
//int count = 1;////tanpa us
int count = 0;/////pakai us
//int state = 0;
char tampung1;
String a = "";
int state=3;
const int pingPin1 = 4;
const int pingPin2 = 3;
String tampung = "";

unsigned long millis1 = 0, millis2 = 0;
//void(* resetFunc)(void)=0;

void setup() {
  //digitalWrite(3,1);
  //pinMode(3, 1);
  // put your setup code here, to run once:
  s1.attach(5);
  s2.attach(6);
  s3.attach(7);
  s4.attach(8);
  s5.attach(9);
  s6.attach(10);
  s7.attach(11);
  pinMode(2, 2);
  pinMode(A0, 2);
  pinMode(A1, 2);
  pinMode(A2, 0);
  pinMode(13, 1);
  s1.write(90);
  s2.write(90);
  s3.write(90);
  s4.write(90);
  s5.write(90);
  s6.write(90);
  s7.write(90);
  Serial.begin(9600);
  // Serial.println("start");

}

void loop() {
  // put your main code here, to run repeatedly:
  
  while (count == 0) {
//    millis1 = millis();
    // if (millis1 - millis2 > 5000) {
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
    //Serial.println(cm);
    // Serial.println(cm2);
    if (cm < 15 ) {
//      s7.write(0);
//      delay(3000);
//      s7.write(90);
//      delay(1000);
//      Serial.print("p");
//      delay(100);
//      count = 1;
      //kedip();
    }
    
    Serial.println(analogRead(A2));
   delay(500);
//
//    millis2 = millis1;

    //}
  }
  while (count == 1) {

    
    //
    //tampung = "";
    //Serial.flush();
//    while (1) {
//      if(Serial.available()){
//        break;
//      }
//
//    }
    state=0;
    while (!Serial.available()) {
      // get the new byte:
     // tampung = Serial.readStringUntil('n');
    }
    //tampung="x";
    while (Serial.available()) {
      // get the new byte:
      tampung = Serial.readStringUntil('n');
    }
   // Serial.print(tampung);
    if (tampung == "a" || tampung == "b" || tampung == "c" || tampung == "d" || tampung == "e"|| tampung == "f") {
      //kedip();
      // Serial.print(tampung);
      if (tampung == "a") {

        s1.write(0);
        motor();
        s1.write(90);
        delay(1000);
      }
      else if (tampung == "b") {
        s2.write(0);
        motor();
        s2.write(90);
        delay(1000);
      } else if (tampung == "c") {
        s3.write(0);
        motor();
        s3.write(90);
        delay(1000);
      } else if (tampung == "d") {
        s4.write(0);
        motor();
        s4.write(90);
        delay(1000);
      } else if (tampung == "e") {
        s5.write(0);
        motor();
        s5.write(90);
        delay(1000);
      } else if (tampung == "f") {
        s6.write(0);
        motor();
        s6.write(90);
        delay(1000);
      }
      //delay(3000);
      // digitalWrite(13,0);
      //mySerial.print(tampung);


      count = 0;//pakai us
      //digitalWrite(3,0);
      tampung="";

     //tampung = "";
//      count = 0;
      state=1;
    }

    
    //count=0;
  }

//  while(count==2){
//    delay(100);
//     count=0;
//    // while (Serial.available()) {
//      // get the new byte:
//      //tampung = Serial.readStringUntil('n');
//    }
   
  



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

void motor() {
  delay(1000);
  digitalWrite(13, 1);
  delay(3000);
  digitalWrite(13, 0);
//  tampung="";
}
