

//Mode Robot
//int debug_stick = 0;

//Servo
#include <Servo.h>
//#define servo_up_pin  2
//#define servo_l_pin   12
//#define servo_r_pin   13
//Servo servo_up;
//Servo servo_l;
//Servo servo_r;
//int servo_left, servo_mid, servo_right;
#include <SoftwareSerial.h>

SoftwareSerial HC12(2, 12); // HC-12 TX Pin, HC-12 RX Pin
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




void setup() {
  pinMode(led_status, OUTPUT);
  Serial.begin(9600);
  HC12.begin(9600);
atas:
  Serial.println("Find Controller");
  error = ps2x.config_gamepad(clk_pin, cmd_pin, att_pin, data_pin, pressures, rumble);
  if (error != 0) {
    delay(300);
    goto atas;
  }
  int type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
    case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
  }

  Serial.println("OK Detected");
  pinMode(pin_dir1, OUTPUT);
  pinMode(pin_dir2, OUTPUT);
  delay(500);


}

void loop() {
  running();
//  HC12.write('U');
//  delay(1000);
//  HC12.write('v');
//  delay(1000);
//  debug_stick();
}



void running() {

  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  read_analog_stick();
  //debug_stick();

    if (key_r1) HC12.write('s');
    else if (key_r2) HC12.write('S');
   // else speed = 100;

  if (key_up || key_analog_up) {
    HC12.write('U');
    Serial.println("u");
  }
  if (key_right || key_analog_right) {
    HC12.write('R');
  }
  if (key_left || key_analog_left) {
    HC12.write('L');
  }
  if (key_down || key_analog_down) {
    HC12.write('D');
  }
  if (key_select) {
    HC12.write('t');
  }
  if (key_l2) {
    HC12.write('i');
  }
  if (key_l1) {
    HC12.write('I');
  }
  if (key_cross) { // buka
    HC12.write('X');
  }
  if (key_square) {
    HC12.write('S');
  }

  if (key_triangle) {
    HC12.write('A');
  }
  if (key_circle) { //buka
    HC12.write('O');
  }
  delay(10);
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
