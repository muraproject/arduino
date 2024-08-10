/*
    ESP-NOW Broadcast Master
    Lucas Saavedra Vaz - 2024

    This sketch demonstrates how to broadcast messages to all devices within the ESP-NOW network.
    This example is intended to be used with the ESP-NOW Broadcast Slave example.

    The master device will broadcast a message every 5 seconds to all devices within the network.
    This will be done using by registering a peer object with the broadcast address.

    The slave devices will receive the broadcasted messages and print them to the Serial Monitor.
*/

#include "DHT.h"
#include "WiFi.h"
#include <esp_now.h>
#include <esp_mac.h>  // For the MAC2STR and MACSTR macros

/* Definitions */

#define DHTPIN 17
#define DHTTYPE DHT11
#define ESPNOW_WIFI_CHANNEL 6

/* Global Variables */

DHT dht(DHTPIN, DHTTYPE); // constructor to declare our sensor
uint32_t msg_count = 0;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Broadcast address

/* Function Prototypes */
void initESPNow();
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status);

/* Main */

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Initialize ESP-NOW
  initESPNow();
}

void loop() {
  delay(1000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed reception");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C, ");
  Serial.print(f);
  Serial.println("°F");

  // Prepare data to send
  char data[64];
  snprintf(data, sizeof(data), "Temp: %.2f°C Hum: %.2f%% #%lu", t, h, msg_count++);

  Serial.printf("Broadcasting message: %s\n", data);

  // Broadcast message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)data, sizeof(data));  // Broadcast address

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(5000);
}

void initESPNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(onSent);

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;  
  peerInfo.encrypt = false;

  // Add broadcast peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
