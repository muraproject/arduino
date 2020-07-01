void setup() {
  // put your setup code here, to run once:
pinMode(3,1);
pinMode(4,1);
pinMode(6,1);
pinMode(7,1);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(3,1);
digitalWrite(4,0);
digitalWrite(6,1);
digitalWrite(7,0);
delay(1000);
digitalWrite(3,0);
digitalWrite(4,1);
digitalWrite(6,0);
digitalWrite(7,1);
delay(1000);
}
