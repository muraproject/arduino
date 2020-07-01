#include <Wire.h>
#include "LiquidCrystal_I2C.h" 
#include <Adafruit_INA219.h> // You will need to download this library

int state=0,pwm,count;

Adafruit_INA219 sensor219;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);// Declare and instance of INA219


byte customChar[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
byte customChar1[] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
byte customChar2[] = {
  B00100,
  B00100,
  B00100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000
};

byte customChar4[] = {
  B00100,
  B00100,
  B00100,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup(void) 
{
  lcd.begin (20,4);
     
  Serial.begin(9600);    
  sensor219.begin();
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  pinMode(3,1);
  pinMode(A0,INPUT_PULLUP);



  ////////////////////////////

   lcd.createChar(0, customChar);
   lcd.createChar(1, customChar1);
   lcd.createChar(2, customChar2);
   lcd.createChar(4, customChar4);
   lcd.setCursor(10,0);
   lcd.write(byte(0));
   lcd.setCursor(10,1);
   lcd.write(byte(0));
    lcd.setCursor(11,0);
   lcd.write(byte(0));
   lcd.setCursor(11,1);
   lcd.write(byte(0));
   lcd.setCursor(10,2);
   lcd.write(byte(2));
    lcd.setCursor(11,2);
   lcd.write(byte(4));
   lcd.setCursor(0,2);
   for(int a=0;a<10;a++){
   lcd.write(byte(1));
   }
   lcd.setCursor(12,2);
   for(int a=0;a<9;a++){
   lcd.write(byte(1));
   }
  ///////////////////////////
  
}

void loop(void) 
{
  float busVoltage = 0;
  float current = 0; // Measure in milli amps
 // float power = 0;

  busVoltage = sensor219.getBusVoltage_V();
  current = sensor219.getCurrent_mA();
  //power = busVoltage * (current/1000); // Calculate the Power
  
  
  //Serial.print("Bus Voltage:   "); 
  Serial.print(busVoltage);
  lcd.setCursor(0,0);
  lcd.print("     "); 
  lcd.setCursor(0,0);
  lcd.print(busVoltage); 
  lcd.setCursor(6,0);
  lcd.print("Volt");
  Serial.println(" V");  
  
 //Serial.print("Current:       "); 
  Serial.print(current); 
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(0,1); 
  lcd.print(current);
  lcd.setCursor(6,1);
  lcd.print("mA");
  Serial.println(" mA");
  
  //Serial.print("Power:         "); 
  //Serial.print(power); 
  //Serial.println(" W");  
  
  Serial.println("");  

  if(state==0){
    digitalWrite(3,1);
    Serial.println("Waiting");
    lcd.setCursor(0,3);
    lcd.print("               ");
    lcd.setCursor(0,3);
    lcd.print("Standby");
    if(digitalRead(A0)==0){
      state=1;
      pwm = 255;
      count = 15;
    }
  }

  if(state == 1){
      Serial.println("inisizlizing...");
      lcd.setCursor(0,3);
      lcd.print("               ");
      lcd.setCursor(0,3);
      lcd.print("initializing..");
    if(current < 300.0 ){
      pwm=pwm-2;
      if(pwm<=0){
        pwm=0;
      }
      analogWrite(3,pwm);
    } else{
      state=2;
    }
    
  }

  if(state==2){
    Serial.println("charging");
    lcd.setCursor(0,3);
    lcd.print("               ");
    lcd.setCursor(0,3);
    lcd.print("charging....");
    Serial.println(pwm);
    analogWrite(3,pwm);
    if(current < 10.0){
      count--;
    }else{
      count=15;
    }
    
    if(current < 10.0 && count < 1 ){
      state=0;
      
    }
   delay(2000);
  }
  
  //analogWrite(3,pwm);
  delay(500);
  
  
}
