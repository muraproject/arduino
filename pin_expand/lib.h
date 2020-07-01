
#define expandPin Serial

void einit() {

  expandPin.begin(9600);


}

int reading() {

  String ch = "";
  int value;
  while (!expandPin.available()) {

  }
  while (expandPin.available()) {
    ch = expandPin.readStringUntil('\n');

  }
  //expandPin.print(ch);
  value = ch.toInt();
  return value;
}

void connection() {
  //boolean del;
  while (!expandPin.available()) {

  }
  while (expandPin.available()) {
    String chk = expandPin.readStringUntil('\n');
  }
  //Serial.flush();

  //Serial.available();

}

int edigitalRead(int pin) {
  int value;
  expandPin.print("d");
  expandPin.println(pin);
  value = reading();
  return value;
}

void edigitalWrite(int pin, int state) {
  int value;
  expandPin.print("w");
  expandPin.print(pin);
  expandPin.print(",");
  expandPin.println(state);
  connection();
}

int eanalogRead(int pin) {
  int value;
  expandPin.print("r");
  expandPin.println(pin);
  value = reading();
  return value;
}

void eanalogWrite(int pin, int state) {
  int value;
  expandPin.print("a");
  expandPin.print(pin);
  expandPin.print(",");
  expandPin.println(state);
}

void epinMode(int pin, int state) {
  int value;
  expandPin.print("p");
  expandPin.print(pin);
  expandPin.print(",");
  expandPin.println(state);
  connection();

}

void setPin(String command) {
  int l = command.length();
  int pin = command.substring(1, l - 2).toInt();
  int state = command.substring(l - 1, l).toInt();
  pinMode(pin, state);
  expandPin.println("1");
}


void digitread(String command) {
  command.remove(0, 1);
  int pin = command.toInt();
  expandPin.println(digitalRead(pin));
}

void digitwrite(String command) {
  int l = command.length();
  int pin = command.substring(1, l - 2).toInt();
  int state = command.substring(l - 1, l).toInt();
  digitalWrite(pin, state);
  expandPin.println("1");
}

void analogread(String command) {
  command.remove(0, 1);
  int pin = command.toInt();
  expandPin.println(analogRead(pin));
}

void analogwrite(String command) {
  int l = command.length();
  int pin = command.substring(1, l - 2).toInt();
  int state = command.substring(l - 1, l).toInt();
  analogWrite(pin, state);
  expandPin.println("1");
}

void eArduino() {

  String ch = "";
  //int value;

  while (expandPin.available()) {
    ch = expandPin.readStringUntil('\n');

  }
  if (ch != "") {
    Serial.println(ch);
    //expandPin.print(ch);
    if (ch.startsWith("p")) { //pin mode
      setPin(ch);
    }

    if (ch.startsWith("d")) { //digitalread
      digitread(ch);
    }
    if (ch.startsWith("w")) { //digitalwrite
      digitwrite(ch);
    }
    if (ch.startsWith("r")) { //analogread
      analogread(ch);
    }

    if (ch.startsWith("a")) { //analogwrite

    }
  }

}
