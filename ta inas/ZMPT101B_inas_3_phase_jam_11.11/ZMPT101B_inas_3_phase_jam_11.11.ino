#include <Adafruit_MLX90614.h>
#include "Wire.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h" 


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

byte pinCurrentR = A0;
byte pinCurrentS = A1;
byte pinCurrentT = A2;

byte pinPhaseR = A3;
byte pinPhaseS = A4;
byte pinPhaseT = A5;

const int sensorInR = A0;
const int sensorInS = A1;
const int sensorInT = A2;

int mVperAmp       = 66;



LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);
float x;
float v;
int n = 1000;
void setup(){
lcd.begin (20,4);
lcd.setBacklightPin(3,POSITIVE);
lcd.setBacklight(HIGH);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);
pinMode(A4,INPUT);
pinMode(A5,INPUT);
mlx.begin();
Serial.begin(9600);
}

void loop()
{

double current_R = getVPP(pinCurrentR);
double current_S = getVPP(pinCurrentS);
double current_T = getVPP(pinCurrentT);

Serial.print("R :  ");
Serial.print(current_R);
Serial.print("A   ");
Serial.print("S :  ");
Serial.print(current_S);
Serial.print("A   ");
Serial.print("T :  ");
Serial.print(current_T);
Serial.print("A   ");

int teganganR = get_volt(pinPhaseR);
int teganganS = get_volt(pinPhaseS);
int teganganT = get_volt(pinPhaseT);

Serial.print("R :  ");
Serial.print(teganganR);
Serial.print("V   ");
Serial.print("S :  ");
Serial.print(teganganS);
Serial.print("V   ");
Serial.print("T :  ");
Serial.print(teganganT);
Serial.print("V   ");

int suhu = mlx.readObjectTempC();
Serial.print("suhu :  ");
Serial.print(suhu);
Serial.println(" *C");

delay(200);


}

double get_volt(const int myvoltage){

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

int rms1=sqrt(vsquare/n);


return rms1;
  
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




