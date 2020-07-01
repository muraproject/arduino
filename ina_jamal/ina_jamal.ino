#include <Wire.h>// memanggil library i2c
#include "LiquidCrystal_I2C.h" // memanggil library lcd
#include <Adafruit_INA219.h> //memanggil library sensor

int state=0,pwm,count; //deklarasi variable

Adafruit_INA219 sensor219; //deklarasi variable sensor
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);//deklarasi pin LCD


byte customChar[] = {  // custom caracter lcd
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
byte customChar1[] = {// custom caracter lcd
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
byte customChar2[] = {// custom caracter lcd
  B00100,
  B00100,
  B00100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000
};

byte customChar4[] = {// custom caracter lcd
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
  lcd.begin (20,4); // lcd yang digunakan 20x4
     
  Serial.begin(9600);  /// baudrate yang digunakan 9600
  TCCR2B = TCCR2B & B11111000 | B00000010; //// frekuensi pwm di tentukan 3khz
  sensor219.begin(); // pengecekan koneksi sensor
  lcd.setBacklightPin(3,POSITIVE);// backligt LCD
  lcd.setBacklight(HIGH);// backligt LCD ON
  pinMode(3,1); // pin 3 sebagai output
  pinMode(A0,INPUT_PULLUP);// pin A0 sebagai input



  //////////////////////////// custom tampilan lcd garis garis

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
  float busVoltage = 0;//// variable nilai tegangan
  float current = 0; // variable nilai arus
 // float power = 0;

  busVoltage = sensor219.getBusVoltage_V(); // mengambil nilai tegangan
  current = sensor219.getCurrent_mA();// mengambil nilai arus
  
  
  
  //Serial.print("Bus Voltage:   "); 
  Serial.print(busVoltage); // menampilkan nilai tegangan di serial monitor
  lcd.setCursor(0,0);
  lcd.print("     "); 
  lcd.setCursor(0,0);
  lcd.print(busVoltage); // menampilkan nilai tegangan di LCD
  lcd.setCursor(6,0);
  lcd.print("Volt");
  Serial.println(" V");  
  
 //Serial.print("Current:       "); 
  Serial.print(current);  // menampilkan nilai arus di serial monitor
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(0,1); 
  lcd.print(current);   //menampilkan nilai arus di LCD
  lcd.setCursor(6,1);
  lcd.print("mA");
  Serial.println(" mA");

  lcd.setCursor(12,0);
  lcd.print("PWM:");
  lcd.setCursor(16,0);
  lcd.print("    ");
  lcd.setCursor(16,0);
  lcd.print(pwm);// menampilkan nilai pwm di LCD
  
  //Serial.print("Power:         "); 
  //Serial.print(power); 
  //Serial.println(" W");  
  
  Serial.println("");  

  if(state==0){  //// jika nilai variable state = 0 maka
    digitalWrite(3,0);
    Serial.println("Waiting");
    lcd.setCursor(0,3);
    lcd.print("               ");
    lcd.setCursor(0,3);
    lcd.print("Standby"); /// kondisi standby
    if(digitalRead(A0)==0){
      state=1;
      pwm = 0;//// nilai pwm 0
      count = 15;
    }
  }

  if(state == 1){  //// jika nilai variable state = 0 maka kondisi inisializing
      Serial.println("inisizlizing...");
      lcd.setCursor(0,3);
      lcd.print("               ");
      lcd.setCursor(0,3);
      lcd.print("initializing..");
    if(current < 70.0 ){ /// jika arus kurang dari 70 ma maka pwm ditambah
      pwm=pwm+2;
      if(pwm>=240){//// pwm tidak bisa lebih dari 240
        pwm=230;
      }
      analogWrite(3,pwm);//// nilai pwm pin 3 tsesuai dengan nilai variable pwm
    } else{
      state=2;
    }
    
  }

  if(state==2){/// jika kondisi variable state = 2 maka kondisi charging
    Serial.println("charging");
    lcd.setCursor(0,3);
    lcd.print("               ");
    lcd.setCursor(0,3);
    lcd.print("charging....");
    Serial.println(pwm);
    analogWrite(3,pwm);/// nilai pwm pin 3 sesuai dengan nilai variable pwm
    if(current < 20.0){//// jika arus kurang dari 20 ma selama 15 detik maka beban dikatakan penuh dan kembali ke posisi stanby
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
