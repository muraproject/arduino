/**
   BasicHTTPClient.ino

    Created on: 24.05.2015
    http://us-central1-muraproject-b57a2.cloudfunctions.net/project/getabsen/?jurusan=MM&id=80fa6fa4

*/

#define SS_PIN D4  //D2
#define RST_PIN D3//D1

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
int flag = 0;
String url;

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
#include <ESP8266WiFi.h> //memasukan library ESP8266
#include "DHT.h" //memasukan library dht11

//mendefinisikan sensor yang dipakai (DHT11, DHT21, dan DHT22)
#define DHTTYPE DHT11   //tipe yang dipilih DHT 11
#define triggerPin  D3
#define echoPin     D4

const int DHTPin = 5;
//inisialisasi library DHTpin
DHT dht(DHTPin, DHTTYPE);

void setup() {

  USE_SERIAL.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("telkom2", "1234567890");
  digitalWrite(LED_BUILTIN,HIGH);

}

void loop() {
  // wait for WiFi connection
  if (flag == 0) {
    cek_absen();
  } else {
    kirim_data();
  }


}


void kirim_data() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    //    int h = dht.readHumidity();
    //    // Baca suhu sebagai Celsius (default)
    //    float tu = dht.readTemperature();

    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    //   url = "http://us-central1-muraproject-b57a2.cloudfunctions.net/project/post/?id=suhuku&suhu=";
    Serial.println(url);


    http.begin(url); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
        flag = 0;
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(3000);
}


void cek_absen() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    //content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  url = "http://us-central1-muraproject-b57a2.cloudfunctions.net/project/absen/?jurusan=MM&id=";
  url += content;
  Serial.println();
  digitalWrite(LED_BUILTIN, HIGH);
  flag = 1;


}
