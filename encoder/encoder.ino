 int hasil;
 double freq;
void setup() {
  // put your setup code here, to run once:
pinMode(3,0);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
cek();


}

void hitung(){
  freq++;
  delay(1);
}

void cek(){
freq=0;
hasil=0;
while(freq<100000){
  hitung();
  if(digitalRead(3)==1){
    while(digitalRead(3)==1){
      hitung();
    }
    hasil++;
   
  }
  else{
    
  }
}
 Serial.println(hasil);
}



