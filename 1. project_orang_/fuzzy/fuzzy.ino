#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Include the header file

#define DS18B20PIN 4
#define BUTTON_PIN 5
#define RESET_PIN 18
#define RELAY_PIN 19
#define SENSOR_PIN A0

OneWire oneWire(DS18B20PIN);
DallasTemperature sensor(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int resetPressed = false;
float temperature;
int moisture, sensor_analog;
int kirim_durasi = 0;
bool buttonPressed = false;
unsigned long buttonPressTime = 0;
#include "online_task.h"
void setup() {
  Serial.begin(115200);
  sensor.begin();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp :");
  lcd.setCursor(0, 1);
  lcd.print("Humy :");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  startOnlineTask();  // Start the task for sending data to the server
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;
    buttonPressTime = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Inisialisasi");
    lcd.setCursor(0, 1);
    lcd.print("Sensor...");
    Serial.println("durasi ");

    // Inisialisasi sensor selama 5 detik
    float totalTemperature = 0;
    int totalMoisture = 0;
    for (int i = 0; i < 5; i++) {
      sensor.requestTemperatures();
      totalTemperature += sensor.getTempCByIndex(0);
      sensor_analog = analogRead(SENSOR_PIN);
      totalMoisture += (100 - ((sensor_analog / 4095.00) * 100));
      delay(1000);  // Delay 1 detik setiap loop
    }
    temperature = totalTemperature / 5;
    moisture = totalMoisture / 5;
    if (moisture == 0) {
      moisture = 1;
    }

    // Hitung durasi penyiraman
    int durasi = durasi_siram(temperature, moisture);
    kirim_durasi = durasi;
    Serial.println(durasi);

    // Tampilkan durasi penyiraman pada LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Durasi = ");
    lcd.print(durasi);
    lcd.print(" s");

    // Nyalakan relay dan tampilkan counter
    digitalWrite(RELAY_PIN, LOW);
    for (int i = 0; i < durasi; i++) {
      lcd.setCursor(0, 1);
      lcd.print("Counter: ");
      lcd.print(i + 1);
      lcd.print(" s");

      sensor.requestTemperatures();
      temperature = sensor.getTempCByIndex(0);
      sensor_analog = analogRead(SENSOR_PIN);
      moisture = (100 - ((sensor_analog / 4095.00) * 100));
      if (moisture == 0) {
        moisture = 1;
      }

      delay(1000);
      if (digitalRead(RESET_PIN) == LOW) {
        resetPressed = true;
        i = durasi;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Penyiraman");
        lcd.setCursor(0, 1);
        lcd.print("Dibatalkan");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp :");
        lcd.setCursor(0, 1);
        lcd.print("Humy :");
        buttonPressed = false;
        kirim_durasi = 0;
      }
      // Delay 1 detik setiap loop
    }
    digitalWrite(RELAY_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp :");
    lcd.setCursor(0, 1);
    lcd.print("Humy :");
    buttonPressed = false;
    kirim_durasi = 0;

    // if (resetPressed) {
    //     resetPressed = false;

    // }
  }

  if (!buttonPressed) {
    sensor.requestTemperatures();
    temperature = sensor.getTempCByIndex(0);
    sensor_analog = analogRead(SENSOR_PIN);
    moisture = (100 - ((sensor_analog / 4095.00) * 100));
    if (moisture == 0) {
      moisture = 1;
    }

    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println("ºC");
    Serial.print("Moisture = ");
    Serial.print(moisture);
    Serial.println("%");

    lcd.setCursor(7, 0);
    lcd.print("      ");
    lcd.setCursor(7, 0);
    lcd.print(temperature);
    lcd.print(" C");

    lcd.setCursor(7, 1);
    lcd.print("      ");
    lcd.setCursor(7, 1);
    lcd.print(moisture);
    lcd.print(" %");
    delay(2000);
  }
}

int durasi_siram(float suhu, float kelembaban) {
  float muDingin = 0, muAgakDingin = 0, muNormal = 0, muPanas = 0;

  if (suhu >= 10 && suhu <= 23) {
    float min1 = (suhu - 10) / (15 - 10);
    float min2 = (23 - suhu) / (23 - 18);
    muDingin = (min1 < min2 ? min1 : min2);
    muDingin = (muDingin > 0 ? muDingin : 0);
  }

  if (suhu >= 19 && suhu <= 27) {
    float min1 = (suhu - 19) / (23 - 19);
    float min2 = (27 - suhu) / (27 - 23);
    muAgakDingin = (min1 < min2 ? min1 : min2);
    muAgakDingin = (muAgakDingin > 0 ? muAgakDingin : 0);
  }

  if (suhu >= 23 && suhu <= 31) {
    float min1 = (suhu - 23) / (27 - 23);
    float min2 = (31 - suhu) / (31 - 27);
    muNormal = (min1 < min2 ? min1 : min2);
    muNormal = (muNormal > 0 ? muNormal : 0);
  }

  if (suhu >= 27 && suhu <= 42) {
    float min1 = (suhu - 27) / (32 - 27);
    float min2 = (42 - suhu) / (42 - 40);
    muPanas = (min1 < min2 ? min1 : min2);
    muPanas = (muPanas > 0 ? muPanas : 0);
  }

  float muKering = 0, muLembab = 0, muBasah = 0;

  if (kelembaban >= 0 && kelembaban <= 30) {
    float min1 = (kelembaban - 0) / (20 - 0);
    float min2 = (30 - kelembaban) / (30 - 20);
    muKering = (min1 < min2 ? min1 : min2);
    muKering = (muKering > 0 ? muKering : 0);
  }

  if (kelembaban >= 28 && kelembaban <= 70) {
    float min1 = (kelembaban - 28) / (55 - 28);
    float min2 = (70 - kelembaban) / (70 - 55);
    muLembab = (min1 < min2 ? min1 : min2);
    muLembab = (muLembab > 0 ? muLembab : 0);
  }

  if (kelembaban >= 68 && kelembaban <= 100) {
    float min1 = (kelembaban - 68) / (85 - 68);
    float min2 = (100 - kelembaban) / (100 - 85);
    muBasah = (min1 < min2 ? min1 : min2);
    muBasah = (muBasah > 0 ? muBasah : 0);
  }

  struct Rule {
    float suhu;
    float kelembaban;
    int output;
  };

  Rule rules[] = {
    { muDingin, muKering, 480 },
    { muAgakDingin, muKering, 720 },
    { muNormal, muKering, 840 },
    { muPanas, muKering, 840 },
    { muDingin, muLembab, 240 },
    { muAgakDingin, muLembab, 480 },
    { muNormal, muLembab, 480 },
    { muPanas, muLembab, 480 },
    { muDingin, muBasah, 120 },
    { muAgakDingin, muBasah, 120 },
    { muNormal, muBasah, 240 },
    { muPanas, muBasah, 240 }
  };

  float maxWeight = 0;
  int output = 0;

  for (Rule rule : rules) {
    float weight = (rule.suhu < rule.kelembaban ? rule.suhu : rule.kelembaban);
    if (weight > maxWeight) {
      maxWeight = weight;
      output = rule.output;
    }
  }

  return output;
}
