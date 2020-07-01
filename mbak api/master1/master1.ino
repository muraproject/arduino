#include <SoftwareSerial.h>

SoftwareSerial data(2,3);
String getdata="";
//char c=0;

void(* ku_reset) (void) = 0;

void setup() {
  
  data.begin(9600);//Using Serial1 Port
  Serial.begin(9600);
  pinMode(8, OUTPUT);//DE/RE Controling pin of RS-485
  pinMode(13, OUTPUT);//DE/RE Controling pin of RS-485
  Serial.println("");
}

void loop() {
  //char getdata='m';

  //digitalWrite(8,HIGH);//DE/RE=HIGH Transmit Enabled M1
  //mySerial.print('9');//Write '9' and Fetch Data From Pro Mini
   
  //digitalWrite(8,LOW);//DE/RE=LOW Receive Enabled M1
 // delay(1000);
  
  if(Serial.available()){ //If Serial Data is available
     
      //digitalWrite(13,1);
    while(Serial.available()){
      char c=Serial.read();
      //getdata += c;
      Serial.print(c);
    }
     // Serial.begin(9600);
     //Serial.println("");
     ku_reset(); 
     
  }

  //delay(20);
}



