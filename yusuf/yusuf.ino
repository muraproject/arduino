#define CW 7 //CW is defined as pin #7//
#define CCW 8 //CCW is defined as pin #8//
#define CW2 9 //CW is defined as pin #7//
#define CCW2 10 //CCW is defined as pin #8//

int x,count,angle_now=50;
String time_now= "08.15",command;
int clock_angle []={ 
  
  50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 
  98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 50 
        
};
String clock_time[] ={

 "07.00","07.15","07.30","07.45","08.00","08.15","08.30","08.45","09.00","09.15","09.30","09.45",
 "10.00","10.15","10.30","10.45","11.00","11.15","11.30","11.45","12.00","12.15","12.30","12.45",
 "13.00","13.15","13.30","13.45","14.00","14.15","14.30","14.45","15.00","15.15","15.30","15.45",
 "16.00","16.15","16.30","16.45","17.00","17.01" 
  
};


int month_angle[] ={

  90, 93, 96, 99, 101, 103, 106, 108, 109, 111, 112, 113, 113, 113, 113, 113, 112, 111, 110, 108, 
  106, 104, 102, 100, 97, 95, 92, 89, 87, 84, 81, 79, 76, 74, 72, 70, 69, 68, 67, 67, 67, 67, 68,
  69, 70, 72, 74, 76, 78, 81, 83, 86, 89 

};

void setup() { //Setup runs once//

  pinMode(CW, OUTPUT); //Set CW as an output//
  pinMode(CCW, OUTPUT); //Set CCW as an output//
  pinMode(CW2, OUTPUT); //Set CW as an output//
  pinMode(CCW2, OUTPUT); //Set CCW as an output//
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  
  Serial.begin(9600); 

}

void loop() { //Loop runs forever//


  ceksensor(30);

//      while (Serial.available()) {
//      char c  = Serial.read();
//      command +=  c;
//       }
//       if(command != ""){
//
//        time_now = command;
//        command="";
//        
//       }
//      run_day();
//      delay(10);


//    Serial.print(clock_time[x] + "    ");
//    Serial.println(clock_angle[x]);
//    delay(1000);
//    x++;
//
//    if(x==42){
//      x=0;
//    }
  
//  ceksensor(30);         //For 1 second//
//  Serial.print("      ");
//  ceksensor1(45);         //For 1 second//

}

void ceksensor( int sudut_tujuan){
  
  float sudut_sekarang=analogRead(A0)*10/25;
  if(sudut_sekarang<sudut_tujuan){
      digitalWrite(CW,HIGH); //Motor runs clockwise//
      digitalWrite(CCW, LOW); //Motor stops//
  }
  else if(sudut_sekarang>sudut_tujuan){
      digitalWrite(CW,LOW); //Motor runs clockwise//
      digitalWrite(CCW, HIGH); //Motor stops//
  }
  else{
      digitalWrite(CW,LOW); //Motor runs clockwise//
      digitalWrite(CCW, LOW); //Motor stops//
  }
  Serial.println(sudut_tujuan);
  delay(50);
  
}

void ceksensor1( int sudut_tujuan1){
  
  float sudut_sekarang1=analogRead(A1)*10/25;
  if(sudut_sekarang1<sudut_tujuan1-3 ){
      digitalWrite(CW2,HIGH); //Motor runs clockwise//
      digitalWrite(CCW2, LOW); //Motor stops//
  }
  else if(sudut_sekarang1>sudut_tujuan1+3 ){
      digitalWrite(CW2,LOW); //Motor runs clockwise//
      digitalWrite(CCW2, HIGH); //Motor stops//
  }
  else{
      digitalWrite(CW2,LOW); //Motor runs clockwise//
      digitalWrite(CCW2, LOW); //Motor stops//
  }
  Serial.println(sudut_sekarang1);
  delay(50);
  
}


void run_day(){

  if(time_now==clock_time[count]){

    angle_now = clock_angle[count];
    
  }
  count++;
  if(count==42){
    count=0;
  }

  ceksensor(angle_now);
  
}

