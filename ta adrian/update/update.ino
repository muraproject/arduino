unsigned long time_now,time_last;
int nilai;


void setup(){
  Serial.begin(9600);
  pinMode(3,INPUT_PULLUP);
  pinMode(A0,0);
  pinMode(9,1);
  
}
void loop(){
  //Serial.print("Time: ");
  
  time_now = millis();
  speed_chek();

  if( time_now-time_last >= 1000){

      Serial.println(nilai);
      nilai=0;
      time_last = time_now;
      analogWrite(9,analogRead(A0)/4);
      
    
  }

  
//delay(200);

  //Serial.println(time);    //prints time since program started            // wait a second so as not to send massive amounts of data
}


void speed_chek(){

  if(digitalRead(3)==0){

    while(digitalRead(3)==0){
      
    }
    nilai++;
  }
  
}

