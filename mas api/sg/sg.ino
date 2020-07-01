

#include <Servo.h>      //including the library of servo motor 
Servo sg90,sg901;             //initializing a variable for servo named sg90
int initial_position = 90,initial_position1 = 90;   //Declaring the initial position at 90
int LDR1 = A0;          //Pin at which LDR is connected
int LDR2 = A1;
int LDR3 = A3;          //Pin at which LDR is connected
int LDR4 = A4;//Pin at which LDR is connected
int error = 5;          //initializing variable for error
int servopin=9;
void setup() 
{ 

  sg90.attach(servopin);// attaches the servo on pin 9
  sg901.attach(11); //pin 11 servo 2
  pinMode(LDR1, INPUT);   //Making the LDR pin as input
  pinMode(LDR2, INPUT);
  sg90.write(initial_position); 
  sg901.write(initial_position1);//Move servo at 90 degree
  delay(2000);            // giving a delay of 2 seconds
}  
 
void loop() 
{ 
  int R1 = analogRead(LDR1); // reading value from LDR 1
  int R2 = analogRead(LDR2);
  int R3 = analogRead(LDR3); // reading value from LDR 1
  int R4 = analogRead(LDR4);// reading value from LDR 2
  int diff1= abs(R1 - R2);   // Calculating the difference between the LDR's
  int diff2= abs(R2 - R1);
  int diff3= abs(R3 - R4);   // Calculating the difference between the LDR's
  int diff4= abs(R4 - R3);
  
  if((diff1 <= error) || (diff2 <= error)) {
    //if the difference is under the error then do nothing
  } else {    
    if(R1 > R2)
    {
      initial_position = --initial_position;  //Move the servo towards 0 degree
      if(initial_position<1){
        initial_position=0;
      }
    }
    if(R1 < R2) 
    {
      initial_position = ++initial_position;
      //Move the servo towards 180 degree
    }
  }
  
  if((diff3 <= error) || (diff4 <= error)) {
    //if the difference is under the error then do nothing
  } else {    
    if(R3 > R4)
    {
      initial_position1 = --initial_position1; 
      if(initial_position1<1){
        initial_position1=0;
      }//Move the servo towards 0 degree
    }
    if(R3 < R4) 
    {
      initial_position1 = ++initial_position1; //Move the servo towards 180 degree
    }
  }
  sg90.write(initial_position);
  sg901.write(initial_position1);// write the position to servo
  delay(100);
  if(initial_position>=179){
    while(initial_position>1){
      initial_position--;
      sg90.write(initial_position);
      delay(100);
    }
  }

  if(initial_position1>=179){
    while(initial_position1>1){
      initial_position1--;
      sg901.write(initial_position1);
      delay(100);
    }
  }
}

