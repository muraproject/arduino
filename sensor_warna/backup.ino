///*********
//  Rui Santos
//  Complete project details at https://randomnerdtutorials.com
//*********/
//#include <Servo.h>
//
//Servo s6;
//// TCS230 or TCS3200 pins wiring to Arduino
//#define S0 4
//#define S1 5
//#define S2 6
//#define S3 7
//#define sensorOut 8
//
//int count = 0;/////pakai us
//
//const int pingPin1 = 4;
//unsigned long millis1 = 0, millis2 = 0;
//// Stores frequency read by the photodiodes 210 184 190kuning
//int redFrequency = 0;
//int greenFrequency = 0;
//int blueFrequency = 0;
//
//// Stores the red. green and blue colors
//int redColor = 0;
//int greenColor = 0;
//int blueColor = 0;
//
//void setup() {
//  // Setting the outputs
//  pinMode(S0, OUTPUT);
//  pinMode(S1, OUTPUT);
//  pinMode(S2, OUTPUT);
//  pinMode(S3, OUTPUT);
//  pinMode(3, OUTPUT);
//  digitalWrite(3, 0);
//
//  s6.attach(10);
//
//  pinMode(2, 1); // fwd
//  pinMode(3, 1); // reverse
//  pinMode(9, 1); /// pwm
//  pinMode(13, 1); /// buzzer
//  pinMode(A0, 2);  // limit buka
//  pinMode(A1, 2);  // limit tutup
//  off();
//
//  s6.write(90);
//  // Setting the sensorOut as an input
//  pinMode(sensorOut, INPUT);
//
//  // Setting frequency scaling to 20%
//  digitalWrite(S0, HIGH);
//  digitalWrite(S1, LOW);
//
//  // Begins serial communication
//  Serial.begin(9600);
//}
//
//void loop() {
//
//
//  while (count == 0) {
//    //    millis1 = millis();
//    // if (millis1 - millis2 > 5000) {
//    // Serial.println("data cek");
//    long duration, duration2, cm, cm2;
//    // for (int i = 0; i < 100; i++) {
//    //  delay(50);
//
//    pinMode(pingPin1, OUTPUT);
//    digitalWrite(pingPin1, LOW);
//    delayMicroseconds(2);
//    digitalWrite(pingPin1, HIGH);
//    delayMicroseconds(5);
//    digitalWrite(pingPin1, LOW);
//    pinMode(pingPin1, INPUT);
//    duration = pulseIn(pingPin1, HIGH);
//    cm = microsecondsToCentimeters(duration);
//
//    delay(50);
//
// 
//    //  }
//
//    //    cm = cm/100.0 ;
//    //    cm2 = cm2/100.0 ;
//    //Serial.println("cm : ");
//    Serial.println(cm);
//    // Serial.println(cm2);
//    if (cm < 15 ) {
//      s6.write(0);
//      digitalWrite(13, 1);
//      delay(3000);
//      s6.write(90);
//      digitalWrite(13, 0);
//      delay(1000);
//
//      //      Serial.print("p");
//      delay(100);
//      count = 1;
//      //kedip();
//    }
//
//    // Serial.println(analogRead(A2));
//    delay(500);
//    //
//    //    millis2 = millis1;
//
//    //}
//  }
//  while (count == 1) {
//    // Setting RED (R) filtered photodiodes to be read
//    digitalWrite(S2, LOW);
//    digitalWrite(S3, LOW);
//
//    // Reading the output frequency
//    redFrequency = pulseIn(sensorOut, LOW);
//    // Remaping the value of the RED (R) frequency from 0 to 255
//    // You must replace with your own values. Here's an example:
//    // redColor = map(redFrequency, 70, 120, 255,0);
//    redColor = map(redFrequency, 476, 8358, 255, 0);
//
//    // Printing the RED (R) value
//    Serial.print("R = ");
//    Serial.print(redColor);
//    delay(100);
//
//    // Setting GREEN (G) filtered photodiodes to be read
//    digitalWrite(S2, HIGH);
//    digitalWrite(S3, HIGH);
//
//    // Reading the output frequency
//    greenFrequency = pulseIn(sensorOut, LOW);
//    // Remaping the value of the GREEN (G) frequency from 0 to 255
//    // You must replace with your own values. Here's an example:
//    // greenColor = map(greenFrequency, 100, 199, 255, 0);
//    greenColor = map(greenFrequency, 583, 3814, 255, 0);
//
//    // Printing the GREEN (G) value
//    Serial.print(" G = ");
//    Serial.print(greenColor);
//    delay(100);
//
//    // Setting BLUE (B) filtered photodiodes to be read
//    digitalWrite(S2, LOW);
//    digitalWrite(S3, HIGH);
//
//    // Reading the output frequency
//    blueFrequency = pulseIn(sensorOut, LOW);
//    // Remaping the value of the BLUE (B) frequency from 0 to 255
//    // You must replace with your own values. Here's an example:
//    // blueColor = map(blueFrequency, 38, 84, 255, 0);
//    blueColor = map(blueFrequency, 844, 309, 255, 0);
//
//    // Printing the BLUE (B) value
//    Serial.print(" B = ");
//    Serial.print(blueColor);
//    delay(100);
//
//    // Checks the current detected color and prints
//    // a message in the serial monitor
//    if (redColor > 100 && redColor < 300 && blueColor < 500) {
//      Serial.println(" kuning");
//      kedip();
//      count=0;
//    } else if (redColor > 200 && redColor < 500 && blueColor < 500) {
//      Serial.println(" kuning");
//      kedip();
//      count=0;
//      
//    } else if (greenColor > 50) {
//      Serial.println(" hijau");
//      while (1) {
//        fwd();
//        if (digitalRead(A0) == 0) {
//          off();
//          break;
//        }
//      }
//
//      while (1) {
//        rvs();
//        if (digitalRead(A1) == 0) {
//          off();
//          break;
//        }
//      }
//      count=0;
//    } else {
//      Serial.println(" gelap");
//    }
//    //  if(redColor > greenColor && redColor > blueColor){
//    //      Serial.println(" - RED detected!");
//    //  }
//    //  if(greenColor > redColor && greenColor > blueColor){
//    //    Serial.println(" - GREEN detected!");
//    //  }
//    //  if(blueColor > redColor && blueColor > greenColor){
//    //    Serial.println(" - BLUE detected!");
//    //  }
//
//  }
//}
//
//
//long microsecondsToCentimeters(long microseconds) {
//  return microseconds / 29 / 2;
//}
//void fwd() {
//  digitalWrite(5, 1);
//  digitalWrite(6, 0);
//}
//void rvs() {
//  digitalWrite(5, 0);
//  digitalWrite(6, 1);
//}
//void off() {
//  digitalWrite(5, 0);
//  digitalWrite(6, 0);
//}
////////
//void kedip() {
//  for (int u = 0; u < 10; u++) {
//    digitalWrite(13, 1);
//    delay(1000);
//    digitalWrite(13, 0);
//    delay(500);
//  }
//}
