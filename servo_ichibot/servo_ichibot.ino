/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo and Micro support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  s,150,50,647
  n,60,120,150
  f,150,50,647

  This example code is in the public domain. s,157,97,64

*/
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(11, 12); // RX, TX
String command = "";
Servo angkat;
Servo capit;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  angkat.attach(6);
  capit.attach(7);
  Serial.println("s,157,97,64");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(19200);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    //Serial.write(Serial.read());
    command = mySerial.readStringUntil('\n');
    Serial.println(command);
    String data1 = command.substring(command.indexOf(',') + 1, command.indexOf(',', 2));
    //    Serial.println(data1);
    String data2 = command.substring(command.indexOf(',', 2) + 1, command.length() - 4);
    //    Serial.println(data2);
    int servo_ang = data1.toInt();
    int servo_cap = data2.toInt();
    Serial.println(servo_ang);
    Serial.println(servo_cap);
    if (command.startsWith("n") > 0) {
      Serial.println("on");
      capit.write(servo_cap);
      delay(1500);
      angkat.write(servo_ang);

    } else if (command.startsWith("s") > 0) {
      Serial.println("set");
      capit.write(servo_cap);
      angkat.write(servo_ang);
    } else {
      Serial.println("off");
      angkat.write(servo_ang);
      delay(1500);
      capit.write(servo_cap);

    }
    command = "";

  }

}
