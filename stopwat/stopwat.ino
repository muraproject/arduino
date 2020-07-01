/*
  Countdown on a single DMD display
*/

#include <SPI.h>
#include <DMD2.h>
#include <fonts/yey.h>
#include <fonts/SystemFont5x7.h>
#include <EEPROM.h>


#include "variable.h"
#include "countdown.h"
#include "stopwatch.h"
#include "kontrol.h"

#include "menu.h"

// the setup routine runs once when you press reset:
void setup() {
  pinMode(10, 1);
  pinMode(2, 0);
  pinMode(3, 0);
  pinMode(4, 0);
  pinMode(5, 0);
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();
  dmd1.setBrightness(255);
  dmd1.selectFont(fontku);
  dmd1.begin();

}

// the loop routine runs over and over again forever:
void loop()
{

  //mundur();
  //mundur1();
  //stopwatch();
  //kontrol();
  //delay(9);
  run();
}
