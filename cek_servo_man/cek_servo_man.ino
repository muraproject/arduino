#include <Servo.h>
Servo satu;
char cek='p';
int nilai=45;

void setup() {
  // put your setup code here, to run once:
satu.attach(A4);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
while(Serial.available()){
  cek = Serial.read();
  
 }

 if(cek!='p'){
  if(cek=='a'){
    nilai=nilai+10;
    satu.write(nilai);
  }
  if(cek=='b'){
    nilai=nilai-10;
    satu.write(nilai);
  }
  Serial.println(nilai);
  cek = 'p';
 }
}
