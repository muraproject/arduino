#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

#define pin_dir1  4
#define pin_dir2  7
#define pin_pwm1  3
#define pin_pwm2  6


#define pin_dir11  A1
#define pin_dir21  A3
#define pin_pwm11  A0
#define pin_pwm21  A2

//LED
#define led_status  0//8

//Global Variable
#define kanan   true
#define kiri    false

int speed=200, speed_l, speed_r;
int speed_l1, speed_r1;
void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);
  pinMode(13, 1); // Serial port to HC12
  Serial.println("OK Detected");
  pinMode(pin_dir1, OUTPUT);
  pinMode(pin_dir2, OUTPUT);
  pinMode(pin_dir11, OUTPUT);
  pinMode(pin_dir21, OUTPUT);

}

void loop() {
  if (HC12.available()) {        // If HC-12 has data
   
    char c = HC12.read();// Send the data to Serial monitor
    Serial.write(c);


    if (c == 's') speed = 150;
    else if (c == 'S') speed = 250;
    //else speed = 100;
    

    if (c == 'U') {
      speed_l = speed_l + speed;
      speed_r = speed_r + speed;
      
    }
    if (c == 'R') {
      if (speed < 80) {
        speed_l = speed_l + (speed * 0.10);
        speed_r = speed_r - (speed * 0.10);
      }
      else {
        speed_l = speed_l + (speed * 0.7);
        speed_r = speed_r - (speed * 0.7);
      }
    }
    if (c == 'L') {
      if (speed < 80) {
        speed_l = speed_l - (speed * 0.10);
        speed_r = speed_r + (speed * 0.10);
      }
      else {
        speed_l = speed_l - (speed * 0.7);
        speed_r = speed_r + (speed * 0.7);
      }
    }
    if (c == 'D') {
      speed_l = speed_l - speed;
      speed_r = speed_r - speed;
    }

    if (c == 'A') {
      pwm_motor1(0, speed);
      
    }
    if (c == 'O') {
      pwm_motor1(0-speed, 0);
    }
    if (c == 'S') {
      pwm_motor1(speed, 0);
    }
    if (c == 'X') {
      pwm_motor1(0, 0-speed);
    }
    
    pwm_motor(speed_l, speed_r);
//    pwm_motor1(speed_l1, speed_r1);
    digitalWrite(13,1);
    Serial.print(speed_l);
    Serial.print("   ");
    Serial.println(speed_r);
    speed_l = 0;
    speed_r = 0;

    if (c == 't') { 
      
    }

    if (c == 'i') {
      
    }
    if (c == 'I') {
      
    }

   
   delay(10);
  }else{
    pwm_motor(0, 0);
    pwm_motor1(0, 0);
    digitalWrite(13,0);
  }

//pwm_motor(230, -230);
//delay(1000);
//pwm_motor(0, 0);
//delay(1000);

}


void pwm_motor(int speed2, int speed1) {
  if (speed1 < 0) {
    speed1 = 255 + speed1;
    digitalWrite(pin_dir1, 1);
  } else {
    digitalWrite(pin_dir1, 0);
  }
  if (speed2 < 0) {
    speed2 = 255 + speed2;
    digitalWrite(pin_dir2, 1);
  } else {
    digitalWrite(pin_dir2, 0);
  }

  if (speed1 > 255) speed1 = 255;
  if (speed2 > 255) speed2 = 255;
  analogWrite(pin_pwm1, speed1);
  analogWrite(pin_pwm2, speed2);
}

void pwm_motor1(int speed2, int speed1) {
  if (speed1 < 0) {
    speed1 = 255 + speed1;
    digitalWrite(pin_dir11, 1);
  } else {
    digitalWrite(pin_dir11, 0);
  }
  if (speed2 < 0) {
    speed2 = 255 + speed2;
    digitalWrite(pin_dir21, 1);
  } else {
    digitalWrite(pin_dir21, 0);
  }

  if (speed1 > 255) speed1 = 255;
  if (speed2 > 255) speed2 = 255;
  analogWrite(pin_pwm11, speed1);
  analogWrite(pin_pwm21, speed2);
}
