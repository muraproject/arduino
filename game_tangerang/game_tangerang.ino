int code = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(A1, 2); //button
  pinMode(A2, 2); // kawat
  pinMode(A3, 2); // finish
  pinMode(13, 1);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(A1) == 0) {
    Serial.print("I");
    while (digitalRead(A1) == 0) {
    }
    code = 1;
    digitalWrite(13, 1);
  }



  if (code == 1) {
    if (digitalRead(A2) == 0) {
      Serial.print("k");
      while (digitalRead(A2) == 0) {
      }
      digitalWrite(13, 0);
      code = 0;
    }
    if (digitalRead(A3) == 0) {
      Serial.print("i");
      while (digitalRead(A3) == 0) {
      }
      digitalWrite(13, 0);
      code = 0;
    }
  }

}
