#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);//RX=2,TX=3 

int input1 = 7; 
int input2 = 8; 
int input3 = 9; 
int input4 = 10; 

int mode1 = 5; 
int mode2 = 6; 

long mytime,lasttime;
int Ms=2000;
String comdata = ""; 
int STATE=1;  

void setup()  
{  
  Serial.begin(9600);  
  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
  pinMode(input3,OUTPUT);
  pinMode(input4,OUTPUT);
    
  pinMode(mode1,OUTPUT);
  pinMode(mode2,OUTPUT);

  digitalWrite(mode1,LOW);
  digitalWrite(mode2,LOW);

 digitalWrite(input1,LOW);
 digitalWrite(input2,LOW);  
 digitalWrite(input3,LOW);
 digitalWrite(input4,LOW);  
 
  mySerial.begin(9600);
}  

void loop()  
{  
  while (Serial.available() > 0)    
  {  
    comdata += char(Serial.read());  
    delay(2);  
  }  
     
  if (comdata.length() > 0)  
  {  
    if(comdata=="A1A1"){
      //forward 
      digitalWrite(input1,HIGH);
      digitalWrite(input2,LOW);
      digitalWrite(input3,HIGH);
      digitalWrite(input4,LOW);   
      Serial.println("B1B1");  
    }
    else if(comdata=="A2A2"){
      //back 
      digitalWrite(input1,LOW);
      digitalWrite(input2,HIGH);  
      digitalWrite(input3,LOW);
      digitalWrite(input4,HIGH);  
      Serial.println("B2B2");  
    }
    else if(comdata=="A3A3"){
      digitalWrite(input1,HIGH);
      digitalWrite(input2,LOW);
      digitalWrite(input3,LOW);
      digitalWrite(input4,LOW);        
      Serial.println("B3B3");  
    }
    else if(comdata=="A4A4"){
      digitalWrite(input1,LOW);
      digitalWrite(input2,LOW);  
      digitalWrite(input3,HIGH);
      digitalWrite(input4,LOW); 
      Serial.println("B4B4");  
    }
    else if(comdata=="A5A5"){
      digitalWrite(input1,LOW);
      digitalWrite(input2,LOW);  
      digitalWrite(input3,LOW);
      digitalWrite(input4,LOW);  
      Serial.println("B5B5");  
    }
     
    STATE=!STATE;  
    digitalWrite(13,STATE);
    comdata = "";  
  }  

  mytime=millis();
  if(mytime-lasttime>=Ms) 
  {
    lasttime=mytime;
    ///Serial.print("hello lora");
  }
}

