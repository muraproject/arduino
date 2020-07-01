
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "speedy telkom 1"; // >>> Nama wifi
char wifiPassword[] = "smartfren0123456789"; // >>> password wifi

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "af998ed0-ff90-11e8-8cb9-732fc93af22b";///ganti
char password[] = "ace2e0cea13686e8e1c1e23cd4ce6abd45461bf6";///ganti
char clientID[] = "f5c67ae0-3ee1-11e9-b67a-4730a9bffc37";///ganti

unsigned long lastMillis = 0;
int LDRs, LDR;
void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
 
}

void loop() {
  Cayenne.loop();

  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  delay(1000);
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_OUT(1)
{
  Cayenne.virtualWrite(1, 90);
}
//CAYENNE_IN(1) {
//  digitalWrite(16, getValue.asInt());
//}
