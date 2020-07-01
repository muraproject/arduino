
#include <SoftwareSerial.h>

#define pwmKa 5
#define cwKa 6
#define ccwKa 7
#define ccwKi 8
#define cwKi 9
#define pwmKi 10
#define foot 12
#define set digitalWrite
#define set_speed analogWrite




String command;

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  pinMode(5,1);// pwm ka
  pinMode(6,1);
  pinMode(7,1);
  pinMode(8,1);
  pinMode(9,1);
  pinMode(10,1);//pwmki
  pinMode(11,1);
  


  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //Serial.println("Hello, world?");
  stoped();
}

void loop() { // run over and over
  while (mySerial.available()) {
    char c  = mySerial.read();
    command +=  c;
  }
  
  if(command != ""){


       //Serial.println(command);
      if(command=="U"){
        Serial.println("Forward");
        forward();
      }
      if(command=="D"){
        Serial.println("Backward");
        backward();
      }
      if(command=="R"){
        Serial.println("Right");
        right();
      }
      if(command=="L"){
        Serial.println("Left");
        left();
      }
      if(command=="N"){
        Serial.println("Stop");
        stoped();
      }
      if(command=="x"){
        Serial.println("Conected");
      }
      if(command=="K"){
        Serial.println("Conected");
        kick();
      }
      command ="";
  }

  
}



void forward(){

  set(cwKa,0);
  set(ccwKa,1);
  set_speed(pwmKa,120);

  set(cwKi,0);
  set(ccwKi,1);
  set_speed(pwmKi,120);
  
}

void backward(){

   set(cwKa,1);
  set(ccwKa,0);
  set_speed(pwmKa,120);

  set(cwKi,1);
  set(ccwKi,0);
  set_speed(pwmKi,120);

  
}

void left(){

  set(cwKa,1);
  set(ccwKa,0);
  set_speed(pwmKa,120);

  set(cwKi,0);
  set(ccwKi,1);
  set_speed(pwmKi,120);
  
}

void right(){

  set(cwKa,0);
  set(ccwKa,1);
  set_speed(pwmKa,120);

  set(cwKi,1);
  set(ccwKi,0);
  set_speed(pwmKi,120);
  
}

void stoped(){

  set(cwKa,0);
  set(ccwKa,0);
  set_speed(pwmKa,0);

  set(cwKi,0);
  set(ccwKi,0);
  set_speed(pwmKi,0);
  
  set(foot,0);
  
}

void kick(){

  set(foot,1);
  
}

