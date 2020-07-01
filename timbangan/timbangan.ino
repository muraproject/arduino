#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
int limit = 100;

void setup() {
  Serial.begin(9600);
  delay(2000);
  pinMode(13,1);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
}

void loop() {

  if (scale.is_ready()) {
    scale.set_scale(9800);
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading/10000);
    Serial.println(analogRead(A3)/2);
    int nilai = reading/10000;
    if(nilai>analogRead(A3)/2){
      digitalWrite(13,1);
    }else{
       digitalWrite(13,0);
    }
   // Serial.print("   ");
    //Serial.print(scale.get_units(50), 4);
   // Serial.println(" kg");
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
  
}
