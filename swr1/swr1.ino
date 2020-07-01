#include <LiquidCrystal.h>

//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte nol[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte satu[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};


byte dua[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte tiga[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte empat[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};


byte lima[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};


void setup() {
  // put your setup code here, to run once:
  pinMode(A3, 0);
  lcd.begin(16, 2);
  lcd.createChar(0, nol);
  lcd.createChar(1, satu);
  lcd.createChar(2, dua);
  lcd.createChar(3, tiga);
  lcd.createChar(4, empat);
  lcd.createChar(5, lima);
}

void loop() {
  // put your main code here, to run repeatedly:
  float analog = (analogRead(A3) * 5 ) / 1023.0;
  float pwr = 2.0 * analog * analog;
  int indaktor = pwr;
  int jumlah = indaktor / 5;
  int sisa = indaktor % 5;
  int count = 0;
  if (pwr >= 1) {
    for (int i = 0; i < jumlah; i++) {
      lcd.setCursor(i, 1);
      lcd.write(5);
      count++;
    }
    lcd.setCursor(count, 1);
    lcd.write(sisa);
    lcd.setCursor(count+1, 1);
    lcd.print("");
  }

  lcd.setCursor(0, 0);
  lcd.print(pwr); lcd.print("W  ");
  lcd.setCursor(13, 0);
  lcd.print("PWR"); 
  lcd.setCursor(11, 1);
  lcd.print(analog); lcd.print("V  ");
  
  delay(1000);
  lcd.clear();

}
