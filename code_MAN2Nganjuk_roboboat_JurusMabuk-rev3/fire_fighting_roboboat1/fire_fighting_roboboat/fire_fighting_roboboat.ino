//#include <Arduino.h>
#include <Servo.h>

// pins
#define echoKiri 7
#define trigKiri 8
#define echoKanan 11
#define trigKanan 12
#define rudderPin 12
#define servoPin      9
#define fanPin        A0
#define rudderNormal 80
#define rudderMin 30
#define rudderMax 135
#define propeller A1
#define pin_MOTOR_PWML 5
#define pin_MOTOR_PWMR 3
#define pin_MOTOR_DIRL1 A3
#define pin_MOTOR_DIRL2 A4
#define pin_MOTOR_DIRR1 A5
#define pin_MOTOR_DIRR2 A1
#define buttonLPin  6
#define buttonRPin  10

//#define kecepatan 11
//#define motorMaju 9
//#define motorMundur 12

// variable
int kp = 5;
int ki = 2;
int kd = 20;
int ts = 10;
int setPoint = 20;
int fanpos = 110;
int speed = 90;
int lastError;
int followMode;
int servoFanState, fanState=0;

Servo fan, rudder;

void setup() {

  //digitalWrite(motorMaju, HIGH);
  //digitalWrite(motorMundur, LOW);
  //analogWrite(kecepatan, 240);
  pinMode(propeller, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(buttonLPin, INPUT_PULLUP);
  pinMode(buttonRPin, INPUT_PULLUP);
  digitalWrite(propeller, LOW);
  digitalWrite(fanPin, HIGH);
  Serial.begin(9600);
  
  fan.attach(servoPin);

  servoFanState = 90;
  fan.write(servoFanState);

  while (1) {
    if (!digitalRead(buttonLPin)) {
      delay(300);
      followMode = 0;
      break;
    }
    if (!digitalRead(buttonRPin)) {
      delay(300);
      followMode = 1;
      break;
    }
  }
}

void loop() {

  //  setMotor(-120,-120);
  if (followMode == 0) {
    follow_wall_kiri();
  }
  else {
    follow_wall_kanan();
  }
  //  int jarakKiri   = get_data(trigKiri, echoKiri);
  //  int jarakKanan  = get_data(trigKanan, echoKanan);
  //  //int besarSudut  = pid(jarakKanan, 200, kp, ki, kd, ts, setPoint);
  //  Serial.print(jarakKiri);
  //  Serial.print("  ");
  //  Serial.println(jarakKanan);

  //  besarSudut = rudderNormal + besarSudut;
  //
  //  if(besarSudut < rudderMin){
  //    besarSudut = rudderMin;
  //  }
  //  else if(besarSudut > rudderMax){
  //    besarSudut = rudderMax;
  //  }
  //
  //  if(jarakKiri<55 && jarakKanan<30){
  //    digitalWrite(fanPin, LOW);
  //    digitalWrite(propeller, LOW);
  //    rudder.write(rudderMax);
  //    for(fanpos = 85; fanpos <= 165; fanpos += 1){
  //      fan.write(fanpos);
  //      delay(30);
  //    }
  //    for(fanpos = 165; fanpos >= 85; fanpos -= 1){
  //      fan.write(fanpos);
  //      delay(30);
  //    }
  //  }
  //  else{
  //    digitalWrite(fanPin, HIGH);
  //    digitalWrite(propeller, HIGH);
  //  }
  //
  //  digitalWrite(propeller, HIGH);
  //  rudder.write(besarSudut);
  //  //fan.write(fanpos);
  //  Serial.println("Kiri: "+String(jarakKiri)+" Kanan: "+String(jarakKanan)+" Sudut: "+String(besarSudut)+"degree");
}
