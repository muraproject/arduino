#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DebugMacros.h"


const char* ssid = "Tenda_F68790";                //Put wifi ssid within the quotes
const char* password = "12345678";         //Put WiFi password within the quotes

const char* host = "script.google.com";
const int httpsPort = 443; //the https port is same

// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout
const char* fingerprint = "";

//const uint8_t fingerprint[20] = {};

String url = String("/macros/s/AKfycbzYQYsWvpe6p1efjayOiF-YOuZBHDgKnen-MEx3tmj14c0zsWfgvUSF51qH5BrZZQh0EA/exec");

HTTPSRedirect* client = nullptr;

// used to store the values of free stack and heap before the HTTPSRedirect object is instantiated
// so that they can be written to Google sheets upon instantiation

void setup() {
  pinMode(D3, 1);
  delay(1000);
  Serial.begin(115200);
 
  Serial.println();
  Serial.print("Connecting to wifi: ");
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

  // Use HTTPSRedirect class to create a new TLS connection
}

void loop() {
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(false);
  client->setContentTypeHeader("application/json");
  Serial.print("Connecting to ");
  Serial.println(host);          //try to connect with "script.google.com"

  // Try to connect for a maximum of 5 times then exit
  bool flag = false;
  for (int i = 0; i < 5; i++) {
    int retval = client->connect(host, httpsPort);
    if (retval == 1) {
      flag = true;
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  if (!flag) {
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }
  
  client->GET(url, host);
  Serial.println(client->getResponseBody());
  String respon = client->getResponseBody();
  if (respon.indexOf("30") != -1) {
    digitalWrite(D3, 1);
    Serial.println("led on");
  }
  if (respon.indexOf("40") != -1) {
    digitalWrite(D3, 0);
    Serial.println("led off");
  }
  Serial.println("\nStart Sending Sensor Data to Google Spreadsheet");


  // delete HTTPSRedirect object
  delete client;
  client = nullptr;
  delay(1000);

}
