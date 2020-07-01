
/*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.

    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/
/*
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "telkom2";
const char* password = "bismillah26";

const char* host = "api.telegram.org";
const int httpsPort = 443;

String line = "";
int sensorSuhu;

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
  pinMode(2, 1);

  // Use WiFiClientSecure class to create TLS connection

}

void loop() {




  ///////////////
  telegram("/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/getUpdates?offset=-1");
  delay(1000);
  if (line.indexOf("suhu") > 0) {
    String suhu = "/bot901305640:AAFs34qeuDPSKezjS6gr62yV3_brTtqIFj0/sendMessage?chat_id=875288625&text=suhu%20sekarang%20:%20";
    suhu += sensorSuhu;
    suhu += "%20derajat";
    telegram(suhu);
    line = "";
  }

  if (line.indexOf("on") > 0) {
    digitalWrite(2, 0);
  }

  if (line.indexOf("off") > 0) {
    digitalWrite(2, 1);
  }
  delay(5000);
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
*/
