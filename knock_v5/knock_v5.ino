//Evan Widloski - 2013-09-06
//Rhythm counter - checks rhythm (input[]) for match to a secret knock
//secret knock works at any tempo

#include <Servo.h>

int device_states[3] = {0,0,0};

//Pin setup
int servoPowerPin = 2;
int servoPin = 3;
int hallPin = 4;
int relay1Pin = 6;
int relay2Pin = 7;


boolean last_doorstate=0;
boolean doorstate=0;
boolean new_seq=false;
Servo doorServo;

void setup(){
    pinMode(hallPin,INPUT);
    pinMode(servoPowerPin,OUTPUT);
    pinMode(relay1Pin,OUTPUT);
    pinMode(relay2Pin,OUTPUT);

    Serial.begin(9600);
    
    doorServo.attach(servoPin);
}

void loop(){
  if(Serial.available()){
    char inBytes[2];
    Serial.readBytesUntil('\n',inBytes,2);
    controlDevice(inBytes[0]-48,inBytes[1]-48);
  }
  
  doorstate=digitalRead(hallPin);
  if(doorstate == 1 && last_doorstate == 0){
    Serial.print("door ");
    Serial.print(doorstate);
    Serial.print(";");
    delay(1500);
    controlDevice(0,0);
  }
  if(doorstate == 0 && last_doorstate == 1){
    Serial.print("door ");
    Serial.print(doorstate);
    Serial.print(";");
  }
  last_doorstate = doorstate;
  

}



void controlDevice(char device_id, char control){
  //control: 0 - on; 1 - off; 2 - toggle
  Serial.print("control ");
  Serial.print(int(device_id));
  Serial.print(int(control));
  Serial.print(";");
  switch (device_id){
    case 0:
      digitalWrite(servoPowerPin,LOW);
      switch(control){
        case 0:
          doorServo.write(20);
          device_states[device_id]=0;
          break;
        case 1:
          doorServo.write(190);
          device_states[device_id]=1;
          break;
        case 2:
          controlDevice(device_id,1-device_states[device_id]);
          break;
      }
      delay(1500);
      digitalWrite(servoPowerPin,HIGH);
      break;
    case 1:
      switch(control){
        case 0:
          digitalWrite(relay1Pin,LOW);
          device_states[device_id]=0;
          break;
        case 1:
          digitalWrite(relay1Pin,HIGH);
          device_states[device_id]=1;
          break;
        case 2:
          controlDevice(device_id,1-device_states[device_id]);
          break;
      }
      break;
    case 2:
      switch(control){
        case 0:
          digitalWrite(relay2Pin,LOW);
          device_states[device_id]=0;
          break;
        case 1:
          digitalWrite(relay2Pin,HIGH);
          device_states[device_id]=1;
          break;
        case 2:
          controlDevice(device_id,1-device_states[device_id]);
          break;
      }
      break;

  }
  tone(6,440,200);        //play ok sound
  return;

}
