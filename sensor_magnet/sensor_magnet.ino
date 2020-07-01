#include<Wire.h>
#include <SoftwareSerial.h>
#define Addr 0x4A
SoftwareSerial mySerial(2, 3);



void setup() {
  // put your setup code here, to run once:
  pinMode(A0, 0);
  pinMode(A1, 0);
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(Addr);
  Wire.write(0x02);
  Wire.write(0x40);
  Wire.endTransmission();
  delay(300);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  if (mySerial.available()) {
  //    char c = mySerial.read();
  //    if (c == 'l') {
  //      float lum = luminan();
  //      mySerial.println(lum);
  //    }
  //
  //    else if (c == 'm') {
  //      mySerial.println(analogRead(A0));
  //    }
  //    else if (c == 'a') {
  //      mySerial.println(analogRead(A1));
  //    }
  //
  //  }
  float lum = luminan();
  Serial.println(lum);
  delay(1000);

}

float luminan() {

  unsigned int data[2];
  Wire.beginTransmission(Addr);
  Wire.write(0x03);
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data luminance msb, luminance lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to lux
  int exponent = (data[0] & 0xF0) >> 4;
  int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);
  float luminance = pow(2, exponent) * mantissa * 0.045;
  return luminance;

}
