#include "SPIFFS.h"
 
void listDir(char * dir){
 
  File root = SPIFFS.open(dir);
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
 
  Serial.println("\n----DIR: /folder1");
  listDir("/folder1");
 
  Serial.println("\n----DIR: /folder2:");
  listDir("/folder2");  
 
  Serial.println("\n----DIR: /folder1/nested:");
  listDir("/folder1/nested");
 
  Serial.println("\n----DIR: /nested:");
  listDir("/nested");
 
}
 
void loop() {}
