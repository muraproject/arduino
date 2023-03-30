#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int buttonNext = 2;
int buttonPause = 3;
int buttonPrevious = 4;

boolean isPlaying = false;

void setup() {
    pinMode(buttonPause, INPUT);
    digitalWrite(buttonPause, HIGH);
    pinMode(buttonNext, INPUT);
    digitalWrite(buttonNext, HIGH);
    pinMode(buttonPrevious, INPUT);
    digitalWrite(buttonPrevious, HIGH);

    pinMode(22,2);
    pinMode(23,2);
    pinMode(24,2);
    pinMode(25,2);
    pinMode(26,2);
    pinMode(27,2);
    pinMode(28,2);
    pinMode(29,2);
    pinMode(30,2);
    pinMode(31,2);
    pinMode(32,2);
    pinMode(33,2);
    pinMode(34,2);
    pinMode(35,2);
    pinMode(36,2);
    pinMode(37,2);
    pinMode(38,2);
    pinMode(39,2);
    pinMode(40,2);
    pinMode(41,2);
    pinMode(42,2);
    pinMode(43,2);
    pinMode(44,2);
    pinMode(45,2);

    mySoftwareSerial.begin(9600);
    Serial.begin(9600);

    delay(1000);

    Serial.println();
    Serial.println("DFPlayer Mini Demo");
    Serial.println("Initializing DFPlayer...");

    if (!myDFPlayer.begin(mySoftwareSerial)) {
        Serial.println("Unable to begin:");
        Serial.println("1.Please recheck the connection!");
        Serial.println("2.Please insert the SD card!");
        while (true);
    }
    Serial.println(F("DFPlayer Mini online."));

    myDFPlayer.setTimeOut(500);

    //----Set volume----
    myDFPlayer.volume(30); //Set volume value (0~30).
    //myDFPlayer.volumeUp(); //Volume Up
    //myDFPlayer.volumeDown(); //Volume Down

    //----Set different EQ----
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
    //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
    //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
    //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
    //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);

    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

//    myDFPlayer.play(1); //Play the first song
//    isPlaying = true;
    Serial.println("Playing..");
}

void loop() {
  if(digitalRead(22)==0){
    myDFPlayer.play(2);
  }
  if(digitalRead(23)==0){
    myDFPlayer.play(3);
  }
  if(digitalRead(24)==0){
    myDFPlayer.play(4);
  }
  if(digitalRead(25)==0){
    myDFPlayer.play(5);
  }
  if(digitalRead(26)==0){
    myDFPlayer.play(6);
  }
  if(digitalRead(27)==0){
    myDFPlayer.play(7);
  }
  if(digitalRead(28)==0){
    myDFPlayer.play(8);
  }
  if(digitalRead(29)==0){
    myDFPlayer.play(9);
  }
  if(digitalRead(30)==0){
    myDFPlayer.play(10);
  }
  if(digitalRead(31)==0){
    myDFPlayer.play(11);
  }
  if(digitalRead(32)==0){
    myDFPlayer.play(12);
  }
  if(digitalRead(33)==0){
    myDFPlayer.play(13);
  }if(digitalRead(34)==0){
    myDFPlayer.play(14);
  }
  if(digitalRead(35)==0){
    myDFPlayer.play(15);
  }
  if(digitalRead(36)==0){
    myDFPlayer.play(16);
  }
  if(digitalRead(37)==0){
    myDFPlayer.play(17);
  }
  if(digitalRead(38)==0){
    myDFPlayer.play(18);
  }
  if(digitalRead(39)==0){
    myDFPlayer.play(19);
  }
  if(digitalRead(40)==0){
    myDFPlayer.play(20);
  }

   if(digitalRead(41)==0){
    myDFPlayer.play(21);
  }
  if(digitalRead(42)==0){
    myDFPlayer.play(22);
  }
  if(digitalRead(43)==0){
    myDFPlayer.play(23);
  }
  if(digitalRead(44)==0){
    myDFPlayer.play(24);
  }
  if(digitalRead(45)==0){
    myDFPlayer.play(25);
  }
//    if (digitalRead(buttonPause) == LOW) {
//        if (isPlaying) {
//            myDFPlayer.pause();
//            isPlaying = false;
//            Serial.println("Paused..");
//        } else {
//            isPlaying = true;
//            myDFPlayer.start();
//            Serial.println("Playing..");
//        }
//        delay(500);
//    }
//    if (digitalRead(buttonNext) == LOW) {
//        if (isPlaying) {
//            myDFPlayer.next();
//            Serial.println("Next Song..");
//        }
//        delay(500);
//    }
//
//    if (digitalRead(buttonPrevious) == LOW) {
//        if (isPlaying) {
//            myDFPlayer.previous();
//            Serial.println("Previous Song..");
//        }
//        delay(500);
//    }
}
