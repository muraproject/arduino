// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#include <PS2X_lib.h>
#include <Servo.h>
Servo angkat;
Servo capit;
Servo putar;
int speed=75;
int cap=10,ang=140,cap1=30,batas_cap= 0,batas_ang=15,batas_cap1=30;

// These are used to set the direction of the bridge driver.
#define ENA 6     //ENA
#define MOTORA_1 7 //IN3
#define MOTORA_2 3 //IN4
#define MOTORB_1 4 //IN1
#define MOTORB_2 5 //IN2
//#define ENB A6      //ENB

#define key_start     (ps2x.Button(PSB_START))
#define key_select    (ps2x.Button(PSB_SELECT))
#define key_up        (ps2x.Button(PSB_PAD_UP))
#define key_right     (ps2x.Button(PSB_PAD_RIGHT))
#define key_left      (ps2x.Button(PSB_PAD_LEFT))
#define key_down      (ps2x.Button(PSB_PAD_DOWN))
#define key_l3        (ps2x.Button(PSB_L3))
#define key_r3        (ps2x.Button(PSB_R3))
#define key_l1        (ps2x.Button(PSB_L1))
#define key_r1        (ps2x.Button(PSB_R1))
#define key_l2        (ps2x.Button(PSB_L2))
#define key_r2        (ps2x.Button(PSB_R2))
#define key_triangle  (ps2x.Button(PSB_TRIANGLE))
#define key_circle    (ps2x.Button(PSB_CIRCLE))
#define key_cross     (ps2x.Button(PSB_CROSS))
#define key_square    (ps2x.Button(PSB_SQUARE))

//#define CTRL_CLK 10
//#define CTRL_BYTE_DELAY 50

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;

void setup(){

 // Configure output pins
 pinMode(ENA, OUTPUT);
 pinMode(MOTORA_1, OUTPUT);
 pinMode(MOTORA_2, OUTPUT);
 //pinMode(ENB, OUTPUT);
 pinMode(MOTORB_1, OUTPUT);
 pinMode(MOTORB_2, OUTPUT);

 // Disable both motors
 digitalWrite(ENA,0);
 //digitalWrite(ENB,0);

 angkat.attach(12);//12
 capit.attach(13);//13
 putar.attach(2);//2
 angkat.write(ang);
 delay(1000);
 capit.write(cap);
 putar.write(cap1);

 // Start serial communication
 Serial.begin(57600);
  atas:
// error = ps2x.config_gamepad(13,11,10,12, true, true);
  error = ps2x.config_gamepad(11,9,10,8, true, true);//setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
// GamePad(clock, command, attention, data, Pressures?, Rumble?)
 // Check for error
 if (error != 0) {
    delay(300);
    Serial.println("cek");
    goto atas;
  }
   
 else if(error == 1)
  Serial.println("No controller found, check wiring or reset the Arduino");
   
 else if(error == 2)
  Serial.println("Controller found but not accepting commands");
  
 else if(error == 3)
  Serial.println("Controller refusing to enter Pressures mode, may not support it.");
   
 // Check for the type of controller
 type = ps2x.readType();
 switch(type) {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    break;
  }
}

// Main loop
void loop(){
   
 if(error == 1) //skip loop if no controller found
  return; 
  
 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate); // disable vibration of the controller

     // Perform movements based on D-pad buttons
     
     // MOVE FORWARD
     if(ps2x.Button(PSB_PAD_UP)) {
        digitalWrite(MOTORA_1,LOW);
        digitalWrite(MOTORA_2,HIGH);
        digitalWrite(MOTORB_1,HIGH);
        digitalWrite(MOTORB_2,LOW);
       // analogWrite(ENB, speed);
        analogWrite(ENA, speed);
        Serial.println("Move forward");
      }
      // TURN RIGHT
      else if(ps2x.Button(PSB_PAD_RIGHT)){
       digitalWrite(MOTORA_1,LOW);
        digitalWrite(MOTORA_2,HIGH);
        digitalWrite(MOTORB_1,LOW);
        digitalWrite(MOTORB_2,HIGH);
        //analogWrite(ENB, speed);
        analogWrite(ENA, speed);
        Serial.println("Turn right");
      }
      // TURN LEFT
      else if(ps2x.Button(PSB_PAD_LEFT)){
         digitalWrite(MOTORA_1,HIGH);
        digitalWrite(MOTORA_2,LOW);
        digitalWrite(MOTORB_1,HIGH);
        digitalWrite(MOTORB_2,LOW);
        //analogWrite(ENB, speed);
        analogWrite(ENA, speed);
        Serial.println("Turn left");
      }
      // MOVE BACK
      else if(ps2x.Button(PSB_PAD_DOWN)){
        digitalWrite(MOTORA_1,HIGH);
        digitalWrite(MOTORA_2,LOW);
        digitalWrite(MOTORB_1,LOW);
        digitalWrite(MOTORB_2,HIGH);
       // analogWrite(ENB, speed);
        analogWrite(ENA, speed);        
        Serial.println("Move back");
      }  
      else {
        digitalWrite(MOTORA_1,0);
        digitalWrite(MOTORA_2,LOW);
        digitalWrite(MOTORB_1,LOW);
        digitalWrite(MOTORB_2,0);
       // analogWrite(ENB, 0);
        analogWrite(ENA, 0);
      }

      if key_square{
        delay(50);
        ang=ang-5;
        if(ang<52){
          ang=52;
        }
        angkat.write(ang);// semakin kecil semakin kebawah
      }
      if key_cross{
        delay(50);
        ang=ang+5;
        if(ang>140){
          ang=140;
        }
        angkat.write(ang);
      }

      
       if key_triangle{
        delay(50);
        cap=cap-5;
        if(cap<10){
          cap=10;
        }
        capit.write(cap);// semakin kecil semakin kebawah
        
        //delay(50);
        cap1=cap1+5;
        if(cap1>115){
          cap1=115;
        }
        putar.write(cap1);
      }
      if key_circle{
        delay(50);
        cap=cap+5;
        if(cap>70){
          cap=70;
        }
        capit.write(cap);
        //delay(50);
        cap1=cap1-5;
        if(cap1<30){
          cap1=30;
        }
        putar.write(cap1);
      }

      if key_l3{
        delay(50);
        cap=cap-5;
        if(cap<30){
          cap=30;
        }
        putar.write(cap);// semakin kecil semakin kebawah
      }
      if key_r3{
        delay(50);
        cap=cap+5;
        if(cap>125){
          cap=125;
        }
        putar.write(cap);
      }

      if key_r1{
        speed=110;
      }
      if key_r2{
        speed=150;
      }
      if key_l1{
        speed=200;
      }
      if key_l2{
        speed=250;
      }
          
 delay(50);
 }    
}
