#include <Adafruit_MLX90614.h>
#include "Wire.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h" 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
byte pinCurrentR    = A0;
byte pinCurrentS    = A1;
byte pinCurrentT    = A2;
byte pinPhaseR      = A3;
byte pinPhaseS      = A4;
byte pinPhaseT      = A5;
const int sensorInR = A0;
const int sensorInS = A1;
const int sensorInT = A2;
int mVperAmp        = 66;
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);
float x;
float v;
int n = 1000;
String url="";

void setup()
{
lcd.begin (20,4);
lcd.setBacklightPin(3,POSITIVE);
lcd.setBacklight(HIGH);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);
pinMode(A4,INPUT);
pinMode(A5,INPUT);
pinMode(A11,INPUT);
pinMode(13,1);
mlx.begin();
Serial.begin(9600);
Serial3.begin(9600);
}

void loop()
{
digitalWrite(13,0);
double current_R = getVPP(pinCurrentR);
double current_S = getVPP(pinCurrentS);
double current_T = getVPP(pinCurrentT);

Serial.print   ("R :  ");
Serial.print   (current_R);
Serial.print   ("A   ");
lcd.setCursor  (0,0);
lcd.print      ("R: ");
lcd.print      (current_R);
lcd.setCursor  (8,0);
lcd.print      ("A");

Serial.print   ("S :  ");
Serial.print   (current_S);
Serial.print   ("A   ");
lcd.setCursor  (0,1);
lcd.print      ("S: ");
lcd.print      (current_S);
lcd.setCursor  (8,1);
lcd.print      ("A ");

Serial.print   ("T :  ");
Serial.print   (current_T);
Serial.print   ("A   ");
lcd.setCursor  (0,2);
lcd.print      ("T: ");
lcd.print      (current_T);
lcd.setCursor  (8,2);
lcd.print      ("A");

int teganganR = get_volt(pinPhaseR);
int teganganS = get_volt(pinPhaseS);
int teganganT = get_volt(pinPhaseT);

Serial.print   ("R :  ");
Serial.print   (teganganR);
Serial.print   ("V   ");
lcd.setCursor  (12,0);
lcd.print      ("R: ");
lcd.print      (teganganR);
lcd.setCursor  (19,0);
lcd.print      ("V");

Serial.print   ("S :  ");
Serial.print   (teganganS);
Serial.print   ("V   ");
lcd.setCursor  (12,1);
lcd.print      ("S: ");
lcd.print      (teganganS);
lcd.setCursor  (19,1);
lcd.print      ("V");

Serial.print   ("T :  ");
Serial.print   (teganganT);
Serial.print   ("V   ");
lcd.setCursor  (12,2);
lcd.print      ("T: ");
lcd.print      (teganganT);
lcd.setCursor  (19,2);
lcd.print      ("V");

int suhu = mlx.readObjectTempC();
Serial.print   ("suhu :  ");
Serial.print   (suhu);
Serial.println (" *C");
lcd.setCursor  (0,3);
lcd.print      ("Object: ");
lcd.print      (suhu);
lcd.setCursor  (13,3);
lcd.print      ("*C");

int cur_R = current_R*100;
int cur_S = current_S*100;
int cur_T = current_T*100;

delay(1000);
url = "";
url +="hghghgh{vr=";
url +=teganganR;
url +="&vs=";
url +=teganganS;
url +="&vt=";
url +=teganganT;
url +="&ar=";
url +=cur_R;
url +="&as=";
url +=cur_S;
url +="&at=";
url +=cur_T;
url +="&t=";
url +=suhu;
url +="}";
Serial.println(analogRead(A11));
if(analogRead(A11)>50){
  digitalWrite(13,1);
  delay(200);
  if(analogRead(A11)>50){
  Serial3.print(url);
  Serial.println(url);
  //Serial.println("0000000000000");
  
  }
 
  while(analogRead(A11)>50){
    
  }
  
  //url = "";
}

}

double get_volt(const int myvoltage)
{
float v2=0;
float vsquare=0;
for (int i=0;i<n;i++)
{
x=(analogRead(myvoltage)-512);
v=(0.00000412*x*x*x-0.000857*x*x+2.675*x-3.198);
v2=v*v;
vsquare=vsquare+v2;
delay(0.1);
}
int rms=sqrt(vsquare/n);
return rms;
}
float getVPP(const int mysensor)
{
float result;
int   readValue;
int   maxValue = 0;
int   minValue = 1024;  
uint32_t start_time = millis();
while((millis()-start_time) < 1000)
{
readValue = analogRead(mysensor);
if (readValue > maxValue)
{
maxValue = readValue;
}
if (readValue < minValue)
{
minValue = readValue;
}
}
result = ((maxValue - minValue) * 5.0) / 1024.0;
double vpp_in   = (result/2.0)*0.707;
double a_in = (vpp_in * 1000)/mVperAmp;
return a_in;
}
