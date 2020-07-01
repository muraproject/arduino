String tampung;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, 2);
  pinMode(A1, 2);
  pinMode(A2, 2);
  pinMode(A3, 2);
  pinMode(A4, 2);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(A0)) {
    Serial.print("L");
    while (!digitalRead(A0)) {

    }
  }
  if (!digitalRead(A1)) {
    Serial.print("N");
    while (!digitalRead(A1)) {

    }
  }
  if (!digitalRead(A2)) {
    Serial.print("l");
    while (!digitalRead(A2)) {

    }
  }
  if (!digitalRead(A3)) {
    Serial.print("n");
    while (!digitalRead(A3)) {

    }
  }
  if (!digitalRead(A4)) {
    Serial.print("p");
    while (!digitalRead(A4)) {

    }
  }

  while (Serial.available()) {
    // get the new byte:
    tampung = Serial.readStringUntil('n');
  }
  if (tampung != "") {
    if (tampung == "b") {
      kedip();
    }
    else if (tampung == "t") {
      kedip1();
    }
    else {

    }


    //mySerial.print(tampung);
    //      count = 0;

    tampung = "";
  }
  // delay(500);
}


void kedip() {
  for (int u = 0; u < 10; u++) {
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
    delay(500);
  }
}
void kedip1() {
  for (int u = 0; u < 5; u++) {
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
    delay(500);
  }
}
