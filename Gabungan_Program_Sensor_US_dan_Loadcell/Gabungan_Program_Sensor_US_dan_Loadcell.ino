#include "HX711.h"
#define DOUT  A0
#define CLK  A1
//IO sensor 1
#define echoPin 11 //Echo Pin
#define trigPin 12 //Trigger Pin
//IO Sensor 2
#define echoPin2 9 //Echo Pin
#define trigPin2 10 //Trigger Pin
//IO Sensor 3
#define echoPin3 2 //Echo Pin
#define trigPin3 4 //Trigger Pin
#include<SoftwareSerial.h>

SoftwareSerial langSerial(3, 5);
const int led = 6, led2 = 7 , led3 = 8;

int maximumRange = 200; //kebutuhan akan maksimal range
int minimumRange = 00; //kebutuhan akan minimal range
long duration, distance1; //waktu untuk kalkulasi jarak

int maximumRange2 = 200;
int minimumRange2 = 00;
long duration2, distance2;

int maximumRange3 = 200;
int minimumRange3 = 00;
long duration3, distance3;

HX711 scale;
float calibration_factor = 650;
int GRAM;

void setup() {
  Serial.begin(9600);
  langSerial.begin(9600);
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.read_average();
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin3, OUTPUT);
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);
  pinMode(13, 1);
  delay(2500);
}
void loop()
{


  /////////////////////////////////////////////////////////////////////////////////////
  char c = 0;
  String url = "";
 // String acc = "";
  while (!langSerial.available()) {

  }
  while (langSerial.available()) {
    c = langSerial.read();

    // acc= langSerial.readStringUntil("|");
    Serial.print(c);
  }
  
   if (c == '1' || c == '0' || c == '2') {
    Serial.println("");
    if (c == '0') {
      digitalWrite(13, 1);
    }
    if (c == '1') {
      digitalWrite(13, 0);
    }

    ////////////////////////////////////////////////////////////////////////////////


    scale.set_offset(142051);
    scale.set_scale(calibration_factor);
    GRAM = scale.get_units(), 4;
    GRAM *= 3;
    GRAM *= 0.86;
    Serial.print("Reading: ");
    Serial.print(GRAM);
    Serial.print(" Gram");
    Serial.println();
    digitalWrite(trigPin, LOW); delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    delay(100);

    digitalWrite(trigPin2, LOW); delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    delay(100);

    digitalWrite(trigPin3, LOW); delayMicroseconds(2);
    digitalWrite(trigPin3, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin3, LOW);
    duration3 = pulseIn(echoPin3, HIGH);
    delay(100);
    //perhitungan untuk dijadikan jarak
    distance1 = duration / 58.2;
    distance2 = duration2 / 58.2;
    distance3 = duration3 / 58.2;
    Serial.print("distance1 = ");
    Serial.print(distance1);
    Serial.println("cm\t");

    Serial.print("distance2 = ");
    Serial.print(distance2);
    Serial.println("cm\t");

    Serial.print("distance3 = ");
    Serial.print(distance3);
    Serial.println("cm");

    if (distance1 == 10) {
      digitalWrite(led, HIGH);
    }
    else if (distance2 == 10) {
      digitalWrite(led2, HIGH);
    }
    else if (distance3 == 10) {
      digitalWrite(led3, HIGH);
    }
    else {
      digitalWrite(led, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      delay(500);
    }

    //////////////////////////////////////////////////
    url += "bahan1=";
    url += distance1;
    url += "&bahan2=";
    url += distance2;
    url += "&bahan3=";
    url += distance3;
    url += "&campuran=";
    url += GRAM;
    url += "|";
    url = "bahan1=7&bahan2=8&bahan3=9&campuran=9|";

    langSerial.print(url);
    Serial.println(url);
  }
}
