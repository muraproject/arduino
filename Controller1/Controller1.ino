

//Mode Robot
//int debug_stick = 0;

//Servo
#include <Servo.h>
#define servo_up_pin  2
#define servo_l_pin   12
#define servo_r_pin   13
Servo servo_up;
Servo servo_l;
Servo servo_r;
int servo_left, servo_mid, servo_right;

//Stik PS
#include <PS2X_lib.h>
#define data_pin  8
#define cmd_pin   9
#define att_pin   10
#define clk_pin   11
#define pressures   false
#define rumble      false
PS2X ps2x;
int error = 0;
byte type = 0;
byte vibrate = 0;

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

int key_analog_up = 0;
int key_analog_down = 0;
int key_analog_left = 0;
int key_analog_right = 0;

struct {
  int lx;
  int ly;
  int rx;
  int ry;
} analog;

//Motor
#define pin_dir1  4
#define pin_dir2  7
#define pin_pwm1  3
#define pin_pwm2  6

//LED
#define led_status  0//8

//Global Variable
#define kanan   true
#define kiri    false
int index = 0;
#define max_index     30
#define max_parameter 10

int data_plan[max_index][max_parameter];

void setup() {
  pinMode(led_status, OUTPUT);
  Serial.begin(57600);
//atas:
//  Serial.println("Find Controller");
//  error = ps2x.config_gamepad(clk_pin, cmd_pin, att_pin, data_pin, pressures, rumble);
//  if (error != 0) {
//    delay(300);
//    goto atas;
//  }
//  int type = ps2x.readType();
//  switch (type) {
//    case 0:
//      Serial.println("Unknown Controller type found ");
//      break;
//    case 1:
//      Serial.println("DualShock Controller found ");
//      break;
//    case 2:
//      Serial.println("GuitarHero Controller found ");
//      break;
//    case 3:
//      Serial.println("Wireless Sony DualShock Controller found ");
//      break;
//  }

  Serial.println("OK Detected");
  pinMode(pin_dir1, OUTPUT);
  pinMode(pin_dir2, OUTPUT);
  delay(500);
  servo_up.attach(servo_up_pin);
  servo_l.attach(servo_l_pin);
  servo_r.attach(servo_r_pin);
 
  index = 0;
}

void loop() {
  pwm_motor(200, 200);
  delay(1000);
  pwm_motor(-200, -200);
  delay(1000);
}

int speed, speed_l, speed_r;

void running() {
next_index:
  //delay(10);
  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  read_analog_stick();
  //debug_stick();

  if (key_start) {
    pwm_motor(data_plan[index][0], data_plan[index][1]);
    delay(data_plan[index][2]);
    pwm_motor(data_plan[index][3], data_plan[index][4]);
    delay(data_plan[index][5]);
    pwm_motor(data_plan[index][6], data_plan[index][7]);
    delay(data_plan[index][8]);
    //key_start = 0;

    while (1) {
      speed = data_plan[index][9];
      //key_start = 0;
      if (data_plan[index][9] == 0) break;
      ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
      read_analog_stick();

      if (key_r1) {
        speed = speed * 1.5;
      }

      if (key_right || key_analog_right) {
        speed_l = speed_l + (speed * 0.5);
        speed_r = speed_r - (speed * 0.5);
      }
      else if (key_left || key_analog_left) {
        speed_l = speed_l - (speed * 0.5);
        speed_r = speed_r + (speed * 0.5);
      }
      else {
        speed_l = speed;
        speed_r = speed;
      }
      pwm_motor(speed_l, speed_r);
      speed_l = 0;
      speed_r = 0;
      delay(5);
      if (key_r2) break;
      if (key_start) {
        index++;
        goto next_index;
      }
    }
    delay(100);
    index++;
  }

  if (key_r1) speed = 150;
  else if (key_r2) speed = 250;
  else speed = 100;

  if (key_up || key_analog_up) {
    speed_l = speed_l + speed;
    speed_r = speed_r + speed;
  }
  if (key_right || key_analog_right) {
    if (speed < 80) {
      speed_l = speed_l + (speed * 0.10);
      speed_r = speed_r - (speed * 0.10);
    }
    else {
      speed_l = speed_l + (speed * 0.7);
      speed_r = speed_r - (speed * 0.7);
    }
  }
  if (key_left || key_analog_left) {
    if (speed < 80) {
      speed_l = speed_l - (speed * 0.10);
      speed_r = speed_r + (speed * 0.10);
    }
    else {
      speed_l = speed_l - (speed * 0.7);
      speed_r = speed_r + (speed * 0.7);
    }
  }
  if (key_down || key_analog_down) {
    speed_l = speed_l - speed;
    speed_r = speed_r - speed;
  }

  pwm_motor(speed_l, speed_r);
  speed_l = 0;
  speed_r = 0;

  if (key_select) {
    servo_up.write(75);
    delay(500);
    servo_l.write(90);
    servo_r.write(10);
  }

  if (key_l2) {
    servo_mid -= 2;
    if (servo_mid > 180) servo_mid = 180;
    servo_up.write(servo_mid);
    delay(2);
  }
  if (key_l1) {
    servo_mid += 5;
    if (servo_mid < 5) servo_mid = 5;
    servo_up.write(servo_mid);
    delay(2);
  }

  if (key_cross) { // buka
    servo_left += 6;
    if (servo_left > 90) servo_left = 90;
    servo_l.write(servo_left);
    delay(5);
  }
  if (key_square) {
    servo_left -= 15;
    if (servo_left < 5) servo_left = 5;
    servo_l.write(servo_left);
    delay(15);
  }

  if (key_triangle) {
    servo_right += 2;
    if (servo_right > 110) servo_right = 110;
    servo_r.write(servo_right);
    delay(5);
  }
  if (key_circle) { //buka
    servo_right -= 10;
    if (servo_right < 10) servo_right = 10;
    servo_r.write(servo_right);
    delay(5);
  }
  delay(10);
}

void set_servo_pos(int mid_pos, int left_pos, int right_pos) {
  servo_up.write(mid_pos);
  servo_l.write(left_pos);
  servo_r.write(right_pos);
}

void read_analog_stick() {
  analog.ly = 128 - (ps2x.Analog(PSS_LY));
  analog.lx = 128 - (ps2x.Analog(PSS_LX));
  analog.ry = 128 - (ps2x.Analog(PSS_RY));
  analog.rx = 128 - (ps2x.Analog(PSS_RX));
  if (analog.ly > 50) key_analog_up = 1;
  else key_analog_up = 0;
  if (analog.ly < -50) key_analog_down = 1;
  else key_analog_down = 0;
  if (analog.lx > 50) key_analog_left = 1;
  else key_analog_left = 0;
  if (analog.lx < -50) key_analog_right = 1;
  else key_analog_right = 0;
}

void debug_stick() {
  if (key_start) Serial.println("Start");
  if (key_select) Serial.println("Select");
  if (key_up) Serial.println("Key Up");
  if (key_right) Serial.println("Key Right");
  if (key_left) Serial.println("Key Left");
  if (key_down) Serial.println("Key Down");
  if (key_l1) Serial.println("L1");
  if (key_l2) Serial.println("L2");
  if (key_r1) Serial.println("R1");
  if (key_r2) Serial.println("R2");
  if (key_triangle) Serial.println("Triangle");
  if (key_circle) Serial.println("Circle");
  if (key_cross) Serial.println("Cross");
  if (key_square) Serial.println("Square");
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
