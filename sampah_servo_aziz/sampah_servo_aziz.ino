#include <Servo.h>

Servo tutup;

void setup() {
  // put your setup code here, to run once:
pinMode(A1,0);
pinMode(13,1);
tutup.attach(9);
Serial.begin(9600);
tutup.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(analogRead(A1));
if(analogRead(A1)>500){
 tutup.write(90); 
 digitalWrite(13,1);
}
else{
  
  tutup.write(15);
 
 digitalWrite(13,0);
}
delay(1000);
}
