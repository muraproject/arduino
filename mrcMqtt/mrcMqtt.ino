

//#include <WiFi.h> //JIKA MENGGUNAKAN ESP32
#include <ESP8266WiFi.h> //JIKA MENGGUNKAN ESP8266
#include <PubSubClient.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D6, D5);
//JIKA MENGG#include <PubSubClient.h>

const char* ssid = "telkom2";
const char* password =  "12345678";//-----Koneksi dengan "cloudmqtt.com"---------------#define mqttServer "NAMA SERVER" //N#define mqttPort 13852 //PORT
#define mqttServer "tailor.cloudmqtt.com"
#define mqttPort 16831
#define mqttUser "ajsgzxba" //NAMA
#define mqttPassword "4bUcaOy-IeI7" //PA//------Deklarasi Variable---------------
char hasil_sensor1[10];
char hasil_sensor2[10];
char hasil_sensor3[10];
//------------------------------------------------
#define led 2
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  //-----Program koneksi dengan wifi---------
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  //-------------------------------------------

  Serial.println("Connected to the WiFi network");
  pinMode(led, OUTPUT);

  //----Program koneksi dengan server cloudmqtt.com...........
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}


void loop() {
  client.loop();
  String mData = "api : ";
  mData += 1023-get_api().toInt();
  mData += ", magnet : ";
  mData += 1023-get_magnet().toInt();
  mData += ", cahaya : ";
  mData += get_luminance().toInt();
  client.publish("Monitoring", mData.c_str());
  Serial.println(mData);
  digitalWrite(2, 0); //Data yang kirimkan harus bertipe data string
  //Data yang kirimkan harus bertipe data string
  delay (1000);





  mData = "";

  mData = "api : ";
  mData += get_api().toInt();
  mData += ", magnet : ";
  mData += get_magnet().toInt();
  mData += ", cahaya : ";
  mData += get_luminance().toInt();
  client.publish("Monitoring", mData.c_str());
  digitalWrite(2, 1); //Data yang kirimkan harus bertipe data string
  //Data yang kirimkan harus bertipe data string
  delay (1000);
  //cara konversi data interger atau float/double ke tipe data char/string
  //x = analogRead(33); Nilai adc dengan tipe data interger
  //dtostrf(x,6,4,hasil_sensor1); syntax konversi ke char/string
  // PENJELASAN (x, 6, 4, hasil_sensor1) :
  // - x = variabel yang akan di konversi
  // - 6 = banyak data
  // - 4 = banyak angka sebelum koma
  // - hasil_sensor1 = hasil konversi tipe data char/string
}




String get_luminance() {
  String tampung = "";
  mySerial.print('l');
  while (!mySerial.available()) {
    delay(1);
  }
  while (mySerial.available()) {
    // get the new byte:
    tampung = mySerial.readStringUntil('\n');
  }
  if (tampung != "") {

  }

  //mySerial.print(tampung);

  return tampung;
}


String get_magnet() {
  String tampung = "";
  mySerial.print('m');
  while (!mySerial.available()) {
    delay(1);
  }
  while (mySerial.available()) {
    // get the new byte:
    tampung = mySerial.readStringUntil('\n');
  }
  if (tampung != "") {

  }

  //mySerial.print(tampung);

  return tampung;
}

String get_api() {
  String tampung = "";
  mySerial.print('a');
  while (!mySerial.available()) {
    delay(1);
  }
  while (mySerial.available()) {
    // get the new byte:
    tampung = mySerial.readStringUntil('\n');
  }
  if (tampung != "") {

  }

  //mySerial.print(tampung);

  return tampung;
}
