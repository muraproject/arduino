void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(3) == 0) {
    Serial.print("x");
    while (digitalRead(3) == 0) {

    }
  }
}
