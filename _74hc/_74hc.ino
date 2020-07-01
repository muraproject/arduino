const int latchPin = 12; // Pin connected to Pin 12 of 74HC595 (Latch)
const int dataPin = 13; // Pin connected to Pin 14 of 74HC595 (Data)
const int clockPin = 11; // Pin connected to Pin 11 of 74HC595 (Clock)
 
// 0,1,2,3,4,5,6,7,8,9
int dec[10] = {63,6,91,79,102,109,125,7,127,111}; //common cathode
 
int count;
int display_number;
 
void setup() {
 
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin, OUTPUT);
 
}
 
void loop() {
 
for (count = 0; count<10; count++){
display_number = dec[count];
digitalWrite(latchPin, LOW);
shiftOut(dataPin, clockPin, MSBFIRST, display_number);
digitalWrite(latchPin, HIGH);
delay(2000);
}
 
}
