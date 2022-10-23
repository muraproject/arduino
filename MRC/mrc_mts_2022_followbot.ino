#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11);
String bluetoothRead, Str_x, Str_p;
int x, points, a;
int b;
int length;
//
//int pwmMotorA = 3;
//int pwmMotorB = 6;
//int ForwardA = 5;
//int BackA = 4;
//int ForwardB = 8;
//int BackB = 7;

#define cwKa 5
#define ccwKa 4
#define ccwKi 7
#define cwKi 6

#define set digitalWrite
#define set_speed analogWrite

int speed = 100;

int bataskiri=400,bataskanan=850;


void setup() {
  BT.begin(9600);
  Serial.begin(9600);

  //  pinMode(pwmMotorA, OUTPUT);
  //  pinMode(ForwardA, OUTPUT);
  //
  //  pinMode(BackA, OUTPUT);
  //  pinMode(pwmMotorA, OUTPUT);
  //
  //  pinMode(ForwardB, OUTPUT);
  //  pinMode(BackB, OUTPUT);



  a = 0;
  b = 0;
  pinMode(4, 1);
  pinMode(5, 1); // pwm ka
  pinMode(6, 1);
  pinMode(7, OUTPUT);



  set(cwKa, 0);
  set(ccwKa, 0);
  set(cwKi, 0);
  set(ccwKi, 0);
}

void loop() {

  //  while (1) {
  //   // set_speed(cwKa, speed + 30);
  ////    set_speed(ccwKa, speed + 30);
  //    //set(cwKa,1);
  //    set_speed(ccwKa, 30);
  //    set_speed(ccwKi, 30);
  //    delay(1000);
  //    Stop();
  //    delay(1000);
  //    set_speed(cwKa, speed+30);
  //    set_speed(cwKi, speed+30);
  //    delay(1000);
  //    Stop();
  //    delay(1000);
  //
  //
  //  }
  int i = 0;
  char commandbuffer[200];

  if (Serial.available()) {
    delay(10);
    while ( Serial.available()) {
      commandbuffer[i++] = Serial.read();
    }

    commandbuffer[i++] = '\0';
    bluetoothRead = (char*)commandbuffer;
    length = bluetoothRead.length();

    if (bluetoothRead.substring(0, 1).equals("X")) {
      int i = 1;

      while (bluetoothRead.substring(i, i + 1) != ("Y")) {
        i++;
      }

      Str_x = bluetoothRead.substring(1, i);
      x = Str_x.toInt();
      //      Serial.print("X: ");
      //      Serial.println(x);

      Str_p = bluetoothRead.substring(length - 1, length);
      points = Str_p.toInt();
      //      Serial.print("Points: ");
      Serial.println(bluetoothRead);
      Serial.println(points);

      i = 1;


      if (x < bataskiri) {
        Serial.println("Left");
        b = 120;
        a = (400 - x) / 3;
        Left();
        delay(80);
        Stop();
        delay(300);
      }
     else if (x > 1000) {
        Serial.println("Right");
        b = 120;
        a = (x - 850) / 3;
        Right();
        delay(80);
        Stop();
        delay(300);

      }


     else if (x < bataskanan && x > bataskiri) {
        //        if (points < 3 && > 0) {
        if (points < 6 && points > 0) {
          Serial.println("Forward");
          a = 0;
          Forward();
          delay(80);
          Stop();
          delay(300);
        }

        else if (points == 6) {
          //          Serial.println("Stop");
          a = 0;
          Stop();
        }
        else {
          //          Serial.println("Back");
          a = 0;
          Back();
        }
      }


      else {
        //eft();
        //         Serial.println("nol");
        Right();
        delay(80);
        Stop();
        delay(40);
        set(13, 1);
        delay(200);
        set(13, 0);
        delay(200);
      }
    }
  }

}


void Left() {
  //  analogWrite(pwmMotorA,b-a);
  //  analogWrite(pwmMotorB, b+a);
  //
  //  digitalWrite(ForwardA, LOW);
  //  digitalWrite(BackA, HIGH);
  //
  //  digitalWrite(ForwardB, HIGH);
  //  digitalWrite(BackB, LOW);
  set_speed(cwKa, speed + 30);
  set_speed(ccwKi, speed + 30);

}


void Right() {
  //  analogWrite(pwmMotorA,b+a);
  //  analogWrite(pwmMotorB,b-a);
  //
  //  digitalWrite(ForwardA, HIGH);
  //  digitalWrite(BackA, LOW);
  //
  //  digitalWrite(ForwardB, LOW);
  //  digitalWrite(BackB, HIGH);

  set_speed(ccwKa, speed + 30);
  set_speed(cwKi, speed + 30);

}


void Forward() {
  //   analogWrite(pwmMotorA,110);
  //  analogWrite(pwmMotorB, 130);
  //  digitalWrite(ForwardA, LOW);
  //  digitalWrite(BackA, HIGH);
  //
  //  digitalWrite(ForwardB, LOW);
  //  digitalWrite(BackB,HIGH);

  set_speed(cwKa, speed + 30);
  set_speed(cwKi, speed + 30);
}


void Back() {
  //  digitalWrite(ForwardA, HIGH);
  //  digitalWrite(BackA, LOW);
  //
  //  digitalWrite(ForwardB, HIGH);
  //  digitalWrite(BackB, LOW);

  set_speed(ccwKa, speed);
  set_speed(ccwKi, speed);
}


void Stop() {

  set(cwKa, 0);
  set(ccwKa, 0);
  set(cwKi, 0);
  set(ccwKi, 0);
}
