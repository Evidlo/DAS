//Evan Widloski - 2013-09-06
//Rhythm counter - checks rhythm (input[]) for match to a secret knock
//secret knock works at any tempo

#include <Servo.h>

int device_states[3] = {0,0,0};

//Device IDs and Commands
#define servoID 0
#define servoOPEN 0
#define servoCLOSE 1
#define servoTOGGLE 2
#define relay1ID 1
#define relay2ID 2

//Pin setup
int servoPowerPin = 2;
int servoPin = 3;
int hallPin = 4;
int relay1Pin = 6;
int relay2Pin = 7;

int debounce_time=100;

boolean doorstate=1;
boolean last_doorstate=doorstate;

boolean new_seq=false;
Servo doorServo;

void setup(){
    pinMode(hallPin,INPUT);
    pinMode(servoPowerPin,OUTPUT);
    pinMode(relay1Pin,OUTPUT);
    pinMode(relay2Pin,OUTPUT);

    Serial.begin(9600);
    
    doorServo.attach(servoPin);
    digitalWrite(servoPowerPin,HIGH);
}

void loop(){
  if(Serial.available()){
    int inByte = Serial.read();
    
    int device_id = inByte / 3;
    int command = inByte % 3;

    controlDevice(device_id,command);
  }
  
  doorstate=digitalRead(hallPin);
  if(doorstate != last_doorstate){
    delay(debounce_time);
    doorstate=digitalRead(hallPin);
  }
  
  if(doorstate == 0 && last_doorstate == 1){
    Serial.print("door ");
    Serial.print(doorstate);
    Serial.print(";");
    delay(1500);
    controlDevice(0,1);
  }
  if(doorstate == 1 && last_doorstate == 0){
    Serial.print("door ");
    Serial.print(doorstate);
    Serial.print(";");
  }
  last_doorstate = doorstate;
  

}

void controlDevice(int device_id,int command){
  Serial.print("control ");
  Serial.print("command ");
  Serial.print(command);
  Serial.print(" device_id ");
  Serial.print(device_id);
  Serial.print(";");
  switch (device_id){
    case servoID:
      digitalWrite(servoPowerPin,LOW);
      switch(command){
        case servoOPEN:
          doorServo.write(10);
          device_states[servoID]=0;
          break;
        case servoCLOSE:
          doorServo.write(240);
          device_states[servoID]=1;
          break;
        case servoTOGGLE:
          controlDevice(servoID,1-device_states[servoID]);
          break;
      }
      delay(1500);
      digitalWrite(servoPowerPin,HIGH);
      break;
    case relay1ID:
     switch(command){
        case 0:
          digitalWrite(relay1Pin,LOW);
          device_states[relay1ID]=0;
          break;
        case 1:
          digitalWrite(relay1Pin,HIGH);
          device_states[relay1ID]=1;
          break;
        case 2:
          controlDevice(relay2ID,1-device_states[relay2ID]);
          break;
      }
    case relay2ID:
      switch(command){
        case 0:
          digitalWrite(relay2Pin,LOW);
          device_states[relay2ID]=0;
          break;
        case 1:
          digitalWrite(relay2Pin,HIGH);
          device_states[relay2ID]=1;
          break;
        case 2:
          controlDevice(relay2ID,1-device_states[relay2ID]);
          break;
      }

  }
  return;

}
