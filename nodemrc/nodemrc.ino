/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "telkom2";
const char* password = "12345678";

#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE);
int h, t;

//WiFiClient client;

// Create an instance of the server
// specify the port to listen on as an argument
IPAddress IP(192, 168, 101, 11);
IPAddress NETMASK(255, 255, 255, 0);
IPAddress NETWORK(192, 168, 101, 1);
IPAddress DNS(8, 8, 8, 8);
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(IP, NETWORK, NETMASK, DNS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val;
  h = dht.readHumidity();
  t = dht.readTemperature();
  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");


  if (req.indexOf("/home") != -1) {
    client.print("<!DOCTYPE HTML>\r\n<html><head> <meta http-equiv='refresh' content='1'<title>MRC 2019</title></head><body>");
    client.print("<H1>");
    client.print(t);
    client.print(" &deg;C</H1></body></HTML>\n");
    delay(1);

  }
  else if (req.indexOf("/req") != -1) {
    client.print("<H1>");
    client.print(t);
    client.print("&deg;C </H1>");
  }

  else if (req.indexOf("/res") != -1) {
    client.print("<H1>");
    client.print(h);
    client.print(" </H1>");
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request




  client.flush();

  delay(1);
  //client.stop();
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}


void reload() {

}

void data() {

  

}
