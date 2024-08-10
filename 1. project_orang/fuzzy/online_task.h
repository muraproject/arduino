#ifndef ONLINE_TASK_H
#define ONLINE_TASK_H

#include <WiFi.h>
#include <HTTPClient.h>
// #include <DallasTemperature.h>

// #define DS18B20PIN 4
// #define SENSOR_PIN A0

// extern float temperature;
// extern int moisture;
// extern DallasTemperature sensor;
// extern int kirim_durasi=0;

void sendDataToServer(void *parameter) {
  // Replace with your actual SSID and password
  WiFi.begin("Tenda_F68790", "12345678");      //Atur Wifi disini ("SSDI", "PASSWORD")
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  while (true) {
    // sensor.requestTemperatures();
    // temperature = sensor.getTempCByIndex(0);
    // int sensor_analog = analogRead(SENSOR_PIN);
    // moisture = 100 - ((sensor_analog / 4095.0) * 100);

    String temp_class = "proses";
    String hum_class = "proses";

    String url = "https://fuzzy.myproject123.com/fuzzy/api/setdata.php?temperature=" + String(temperature) + 
                 "&temp_class=" + temp_class + "&humidity=" + String(moisture) + "&hum_class=" + hum_class + 
                 "&watering="+String(kirim_durasi)+ "&setmotor=cek";
                 
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.println("Data sent successfully");
      } else {
        Serial.println("Error sending data: " + String(httpResponseCode));
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    delay(3000); // Wait for 3 seconds before sending the next data
  }
}

void startOnlineTask() {
  xTaskCreatePinnedToCore(
    sendDataToServer,   /* Function to implement the task */
    "SendDataTask",     /* Name of the task */
    10000,              /* Stack size in words */
    NULL,               /* Task input parameter */
    1,                  /* Priority of the task */
    NULL,               /* Task handle. */
    1                   /* Core where the task should run */
  );
}

#endif
