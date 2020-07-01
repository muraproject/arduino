
////https://api.telegram.org/bot820071304:AAFTMdl67nUUKD_e7fT1YGyUNgPknTI0Ojg/getUpdates?offset=-1 cari chat id
//// https://api.telegram.org/bot820071304:AAFTMdl67nUUKD_e7fT1YGyUNgPknTI0Ojg/sendMessage?chat_id=824864437&text=yeaahhh kirim data

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"

#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//#define D5 5;

const char* ssid = "DESKTOP-JECMJ60 0350";
const char* password = "bismillahbisa";

const char* host = "api.telegram.org";
const int httpsPort = 443;

String line = "";


// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(D0, OUTPUT);
  pinMode(4, OUTPUT);
  dht.begin();

  // Use WiFiClientSecure class to create TLS connection
}

void loop() {

  float sensorSuhu = dht.readTemperature(); //pembacaan sensor suhu
  float kelembaban = dht.readHumidity(); // pembacaan kelembaban

  ///////////////
  telegram("/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/getUpdates?offset=-1");
  delay(1000);
  if (line.indexOf("SUHU") > 0) {
    String suhu = "/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/sendMessage?chat_id=875288625&text=Suhu%20Sekarang%20:%20";
    suhu += sensorSuhu;
    suhu += "%20derajat";
    telegram(suhu);
    line = "";
  }
  if (line.indexOf("KELEMBABAN") > 0) {
    String kelembaban = "/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/sendMessage?chat_id=875288625&text=Kelembaban%20Sekarang%20:%20";
    kelembaban += kelembaban;
    kelembaban += "%20%";
    telegram(kelembaban);
    line = "";
  }
  if (line.indexOf("OFF") > 0) {  //off kipas
    digitalWrite(D0, HIGH);
    String modeOFF = "/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/sendMessage?chat_id=875288625&text=Fan%20Mode%20OFF%20";
    telegram(modeOFF);
    line = "";
  }
 
    

//  if (line.indexOf("ON_B") > 0) {   //on blower
//    digitalWrite(4, HIGH);
//  }
 
   if (line.indexOf("ON") > 0) {   // on kipas
    digitalWrite(D0, LOW);
    String modeON = "/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/sendMessage?chat_id=875288625&text=Fan%20Mode%20ON%20";
    telegram(modeON);
    line = "";
    }
    
//  if (line.indexOf("OFF_B") > 0) {  //off blower
//    digitalWrite(4, LOW);
//  }
//  else{
//    digitalWrite(D0, HIGH);
//  }
  delay(500);digitalWrite(D0, HIGH);
}


void telegram( String mUrl) {
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = mUrl;
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  line = client.readStringUntil(']');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");

  //if(line.equals("robot")){
  //callback();
  //}
}
