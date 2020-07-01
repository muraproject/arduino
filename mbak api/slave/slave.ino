#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

void setup() {
    
 // Serial1.begin(9600);//Uncomment for Arduino Lenardo
  Serial.begin(9600);
  mySerial.begin(9600);
  //while(!Serial1);//Uncomment for Arduino Lenardo http://192.168.4.1

  pinMode(13, OUTPUT);//Led Connected
  pinMode(8, OUTPUT);//DE/RE Controling pin of RS-485
  pinMode(A0,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  //mySerial.print('1');
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(A0)){
    digitalWrite(8,1);
    
    mySerial.print("1");
    while(!digitalRead(A0)){
      
    }
    //delay(100);
    digitalWrite(8,0);
  }
  
  if(!digitalRead(A1)){
    digitalWrite(8,1);
    
    mySerial.print("2");
    while(!digitalRead(A1)){
      
    }
    //delay(100);
    digitalWrite(8,0);
  }

  if(!digitalRead(A2)){
    digitalWrite(8,1);
    
    mySerial.print("3");
    while(!digitalRead(A2)){
      
    }
    //delay(100);
    digitalWrite(8,0);
  }
}
