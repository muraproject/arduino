#include <SoftwareSerial.h>

SoftwareSerial ss(5,4);

void setup()
{
  Serial.begin(4800);
  ss.begin(9600);
}

void loop()
{
  if (ss.available())
    Serial.print((char)ss.read());
  if (Serial.available())
    ss.print((char)Serial.read());
}

