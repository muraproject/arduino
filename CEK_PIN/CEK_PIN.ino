void setup() {
  // put your setup code here, to run once:
pinMode(D7,INPUT_PULLUP);
//digitalWrite(6,0);
pinMode(D6,1);
pinMode(D1,1);
pinMode(D2,1);
pinMode(2,1);
}

void loop() {
 if(digitalRead(D7)==1){

  digitalWrite(D6,0);
  digitalWrite(D1,0);
  digitalWrite(D2,0);
  digitalWrite(2,0);
 }
 else{

   digitalWrite(D6,1);
   digitalWrite(D1,1);
   digitalWrite(D2,1);
   digitalWrite(2,1);
 }
}
