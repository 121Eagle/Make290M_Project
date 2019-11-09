#include<ESP32Servo.h>
#include<Wire.h>
Servo servo1,servo2;
const int distance = 5;
int movement = 0;
void setup() {
  int pos = 0;
  const int BAUD_RATE = 230400;
  // put your setup code here, to run once:
  servo1.attach(23), servo2.attach(18);
  Serial.begin(BAUD_RATE);
  servo1.write(0), servo2.write(0);
}

void moveServo(int value){
  servo1.write(value);
  servo2.write(value);
  delay(15); //TO REMOVE
}
double servoPosition(){
  return servo1.read();
}

void loop() {
  byte directionArray[2];
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    Serial.readBytes(directionArray,2);
  }
  if(directionArray[0] == 1){
    movement = distance;
  }else if (directionArray[1] == 1){
    movement = -distance;
  }
  moveServo(servoPosition() + movement);
}
