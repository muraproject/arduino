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
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004T.h>

#include <ESP8266HTTPClient.h>
#include "url.h"

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;



PZEM004T pzem(12,14);  // D6, D5, (RX,TX) connect to TX,RX of PZEM
IPAddress ip(192,168,1,1);
float arus, tegangan, daya, kwh;

void writeString(char add,String data);
String read_String(char add);
void ap_mode();
int _mode,count_=0;
int econd,kondisi,_onstate,mOnstate;
int compare;
int _restart;
int cond_;

// Replace with your network credentials
const char* ssid     = "IOT_KWH";
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


      pinMode(D7,INPUT_PULLUP);
     // pinMode(2,1);
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
      Serial.begin(9600);
      pzem.setAddress(ip);
      
    
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
    digitalWrite(2,0);
    while(digitalRead(D7) == 0 && count_ < 5001){

    count_++;
    delay(1);
      
    }

    if(digitalRead(D7) == 0 && count_ >= 5000){
    //EEPROM.write(120, econd);
    eeprom_clear(0,110);
    Serial.print("esp restart");
    write_eeprom(100,"AP");
    digitalWrite(2,0);
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

  unsigned long timer = millis();
  if(timer>120000){
    ESP.restart();
  }
  
  tegangan = pzem.voltage(ip);
  if (tegangan < 0.0) tegangan = 0.0;
  //Serial.print(tegangan);Serial.print("V; ");
 
  arus = pzem.current(ip);
  if(arus >= 0.0){ /*Serial.print(arus);Serial.print("A; ");*/ }else{arus =0.0;}
   
  daya = pzem.power(ip);
  if(daya >= 0.0){/* Serial.print(daya);Serial.print("W; ");*/ }else{daya =0.0;}
   
  kwh = pzem.energy(ip);
  if(kwh >= 0.0){/* Serial.print(kwh);Serial.print("Wh; "); */}else{kwh =0.0;}


  int volt = tegangan*100;
  int current_ = arus*100;
  int power_ = daya*100;
  int consume_ = kwh;
  String url_ = "http://iot.muraproject.com/php/just.php?mId=";
         url_ += "hjui";
         url_ += "&mC=";
         url_ += volt;
         url_ += ",";
         url_ += current_;
         url_ += ",";
         url_ += power_;
         url_ += ",";
         url_ += consume_;
 // Serial.print(url_);
 // Serial.println();

   if ((WiFiMulti.run() == WL_CONNECTED)) {

    

    HTTPClient http;
    
    String _url = url_;
    
   // USE_SERIAL.print("[HTTP] begin...\n");
//    if(mOnstate == 1){
//     if(kondisi == 1){
//       _url = "http://iot.muraproject.com/php/frommikro.php?mId=poiuy&mC=1";
//     }
//     else{
//       _url = "http://iot.muraproject.com/php/frommikro.php?mId=poiuy&mC=0";
//      
//     }
//    
///   Serial.println(_url);
//   
//    }else{
//    _url = "http://iot.muraproject.com/php/frommikro.php?mId=poiuy&mC=q"; 
//    }
    // configure server and url
    http.begin(_url);
    //http.begin("192.168.1.12", 80, "/test.html");

   // USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
     // USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
       //digitalWrite(2,0);
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
           // Serial.write(buff, c);
            for (int i = 0; i < 1; i++) {
                
                //Serial.print((char)buff[i]);
               // status_ = (char)buff[i];
                
               
            }
           
            //status_ = String(status_ + buff);
            //Serial.println(status_);
           // status_ +=c;
           if (c==0 || c == 1){
            // status_ = String(c);
            }

            if (len > 0) {
              len -= c;
            }
            
            //digitalWrite(2,c);
          } 
          delay(1);
          

          //cond_ = status_.toInt();
          //status_ = "";
           //digitalWrite(2,cond_);
           //Serial.print("data masuk  ");
         //Serial.println(cond_);
          
        }

        //USE_SERIAL.print("[HTTP] connection closed or file end.\n");
       

        //   _restart++;
        // if (_restart >= 20){
        //   ESP.deepSleep(15e6); 
         //}
         
        
        
        
      }

      //Serial.println(cond_);
    } else {
      //USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    //delay(5000);
  }

   delay(2000);
}
