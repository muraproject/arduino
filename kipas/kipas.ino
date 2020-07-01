int state=0;
void setup() {
  // put your setup code here, to run once:
pinMode(A0,0);
pinMode(A1,INPUT_PULLUP);
pinMode(8,1);
pinMode(9,1);

}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(A0)==1 && digitalRead(A1)==0){
  digitalWrite(8,1); 
  delay(1500);
  digitalWrite(8,0);
  digitalWrite(9,1);
  while(digitalRead(A0)==1 && digitalRead(A1)==0){
   digitalWrite(9,1); 
  }
  digitalWrite(9,0);
  delay(200);
}

}
