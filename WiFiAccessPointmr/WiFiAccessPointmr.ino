/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#include "url.h"


void writeString(char add,String data);
String read_String(char add);
void ap_mode();
int _mode,count_=0;
int econd,kondisi,_onstate,mOnstate;
int compare;
int _restart;

// Replace with your network credentials
const char* ssid     = "IOT_";
const char* password = "muraproject";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String esid;
String epass;

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
#include "AP_.h"
void setup() {
  
  EEPROM.begin(1024);

  // Connect to Wi-Fi network with SSID and password

 //writeString(10, "default");
// writeString(50, "default");

//  delay(1000);
  esid = read_eeprom(0,31);
  //Serial.print("SSID: ");
  //Serial.println(esid);
  epass = read_eeprom(32,96);
  //Serial.print("PASS: ");
  //Serial.println(epass); 
  String estate = read_eeprom(100,110); 
  //Serial.print("State: ");
  //Serial.println(estate); 

  if(estate=="client"){

   Serial.begin(9600);
   // delay(2000);
    pinMode(2,1);
    pinMode(D7,INPUT_PULLUP);
    pinMode(D6,1);
    pinMode(D1,1);
    pinMode(D2,1);
    digitalWrite(2,1);
    digitalWrite(D1,0);
    delay(200);
    digitalWrite(2,0);
    delay(200);
    digitalWrite(2,1);
    digitalWrite(D1,0);
    delay(200);
    digitalWrite(2,0);
    //digitalWrite(D2,0);
    if(digitalRead(D7) == 0 ){
      kondisi = int(EEPROM.read(120));
    //  Serial.println(kondisi);
      //delay(500);
      if(kondisi == 1 ){
  
        econd = 0;
        //digitalWrite(2,1);
        //digitalWrite(D1,1);
        //digitalWrite(D2,0);
        //Serial.print("kondisi "); 
         digitalWrite(D1,0);
        
      }
  
      else{
  
        econd = 1;
        //digitalWrite(2,0);
        //digitalWrite(D1,0);
        //digitalWrite(D2,1);
        //Serial.print("kondisi "); 
        //Serial.println(kondisi); 
        digitalWrite(D1,0);
      }
       digitalWrite(D1,0);

      EEPROM.write(120, econd);
      delay(100);
      EEPROM.write(125, 1);
      EEPROM.commit();
     // Serial.println("finsh_write"); 
      digitalWrite(D6,1);
      delay(500);
      //digitalWrite(D6,0);
      //delay(500);
      ESP.restart();
     
     

   }
   else{

     kondisi = int(EEPROM.read(120));
     _onstate =  int(EEPROM.read(125));
     if(_onstate == 1){

      mOnstate = 1;
      EEPROM.write(125, 0);
      EEPROM.commit();
      
     }else{

      
     }
     //Serial.println(kondisi);
      //delay(500);
      if(kondisi == 1 ){
  
        //econd = 0;
        digitalWrite(2,1);
        digitalWrite(D1,1);
        digitalWrite(D2,0);
        //Serial.print("kondisi "); 
        //Serial.println(kondisi); 
        
      }
  
      else{
  
       // econd = 1;
        digitalWrite(2,0);
        digitalWrite(D1,0);
        digitalWrite(D2,1);
        //Serial.print("kondisi "); 
        //Serial.println(kondisi); 
        
      }
   }
   delay(2000);
      
      WiFi.mode(WIFI_STA);
      //Serial.println("");
      //Serial.print(esid);
      //Serial.print(epass);

      char __esid[esid.length()+1]; esid.toCharArray(__esid, sizeof(__esid));
      char __epass[epass.length()]; epass.toCharArray(__epass, sizeof(__epass));
      //Serial.print(__esid);
      //Serial.print(__epass);
      
      const char* ssku = __esid;
      const char* ppku = __epass;
     // Serial.print(ssku);
      //Serial.print(ppku);
      //Serial.print("_________________________");
      
      
      WiFiMulti.addAP(ssku, ppku);
      
    
     _mode = 1;
   
   
    
 

    
    
    
  }
  else{

    _mode = 0;
    
     
  // Serial.begin(115200);
  //Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  
  server.begin();
  delay(2000);
    
  }

  
}

void loop(){

if(_mode ==0){
  ap_mode();
}else{

  client_mode();
  
}
    
 
}







void client_mode(){


   
  if(digitalRead(D7) == 0 && count_ < 5001){

    
   
   // Serial.print("esp change");
    digitalWrite(2,econd);
    while(digitalRead(D7) == 0 && count_ < 5001){

    count_++;
    delay(1);
      
    }

    if(digitalRead(D7) == 0 && count_ >= 5000){
    //EEPROM.write(120, econd);
    eeprom_clear(0,110);
    //Serial.print("esp restart");
    write_eeprom(100,"AP");
    //digitalWrite(2,kondisi);
    delay(1000);
    ESP.restart();
    }

    
  }
  else{
    req();
    count_ = 0;
    //digitalWrite(2,1);
  }
 
  
}


void req(){

   if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;
    String _url;
     Serial.print("[HTTP] begin...\n");
    if(mOnstate == 1){
     if(kondisi == 1){
       _url = "http://iot.muraproject.com/php/frommikro.php?mId=poiuy&mC=1";
     }
     else{
       _url = "http://iot.muraproject.com/php/frommikro.php?mId=poiuy&mC=0";
      
     }
    
    //Serial.println(_url);
   
    }else{
    _url = "http://iot.muraproject.com/php/frommikro.php?mId=poiuy&mC=q"; 
    }
    // configure server and url
    http.begin(_url);
    //http.begin("192.168.1.12", 80, "/test.html");

   // USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
     // USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get lenght of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();

        // create buffer for read
        uint8_t buff[128] = { 0 };

        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();
          String status_;
          if (size) {
            // read up to 128 byte
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            //USE_SERIAL.write(buff, c);
            //for (int i = 0; i < 3; i++) {
                
                //Serial.println((char)buff[i]);
                
                status_ = (char)buff[1];
                
               
            //}
           
            //status_ = String(status_ + buff);
            Serial.println(status_);
           // status_ +=c;
          

            if (len > 0) {
              len -= c;
            }
            
            //digitalWrite(2,c);
          } 
          delay(1);
          //Serial.println(esid);
          //Serial.println(epass);
         int cond_ = status_.toInt();
         if( cond_ == compare){
         
         }else{
          
          EEPROM.write(120, cond_);
          delay(100);
          EEPROM.commit();
          compare = cond_;


           if(cond_ == 1){
          digitalWrite(D1,1);
          digitalWrite(D2,0);
         } if(cond_ == 0){

          digitalWrite(D1,0);
          digitalWrite(D2,1);
          
         }
//         delay(1000);
//         digitalWrite(D1,0);
//         digitalWrite(D2,0);
         // Serial.print("");
         // Serial.println(cond_);
         digitalWrite(2,cond_);
         delay(1);
          
         }
        
         //digitalWrite(3,cond_);
         //digitalWrite(1,cond_);
         mOnstate = 0;
         // USE_SERIAL.print("msuk : ");
          //USE_SERIAL.println(status_);
        status_ = "";
         _restart++;
         if (_restart >= 20){
           ESP.deepSleep(15e6); 
         }
        }

        

        //USE_SERIAL.print("[HTTP] connection closed or file end.\n");

      }
      
    } else {
     // USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    
    
  }

   delay(5000);
}
