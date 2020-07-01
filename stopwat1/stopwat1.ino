/*
  Countdown on a single DMD display
 */

#include <SPI.h>
#include <DMD2.h>
#include <fonts/yey.h>
#include <fonts/SystemFont5x7.h>

int menit=5;
int detik=127;
int cond =0;
unsigned long allSeconds=0;
unsigned long lastmil;



SoftDMD dmd(1,1);  // DMD controls the entire display
DMD_TextBox box(dmd, 0, 0);  // "box" provides a text box to automatically write to/scroll the display
DMD_TextBox boxDetik(dmd, 19, 1);
DMD_TextBox boxMenit(dmd, 4, 1);
DMD_TextBox boxTitik(dmd, 15, 1);
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();
  boxTitik.print(":");
  boxMenit.print("0");
  boxMenit.print(menit);
  //box.print("99:66");
}

// the loop routine runs over and over again forever:
void loop() {
 countdown();



  
}

void countdown(){
 if(cond==0){
   detik--;
   boxDetik.clear();
   if(detik<10){
    boxDetik.print("0"); 
   }
   boxDetik.print(detik);


   
   if(menit==0 && detik==0){
     dmd.fillScreen(true);
     delay(500);
     dmd.clearScreen();
     delay(500);
     dmd.selectFont(SystemFont5x7);
     DMD_TextBox box(dmd, 5, 3);
     box.print("stop");
     cond = 1;
   }


   
   if(detik==0 && cond == 0){
    detik=0;
    menit++;
    boxMenit.clear();
    if(menit<10){
     boxMenit.print("0"); 
    }
    boxMenit.print(menit);
    boxTitik.clear();
    boxTitik.print(":");
    //boxDetik.clear(); 
    //boxDetik.print("0");
    //boxDetik.print(detik);  
   }
   delay(1000);
 }
}
