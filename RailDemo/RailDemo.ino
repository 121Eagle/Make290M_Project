#include<ESP32Servo.h>

Servo servo1;

void setup() {
  const int BAUD_RATE = 230400;
  servo1.attach(23);
  Serial.begin(BAUD_RATE);
  servo1.write(90);
}

unsigned long time_to_cross = 10000.0; //time it takes to cross in millis

//Servo 1 variables
unsigned long pos_1 = 50.0; //x coord, out of 100
int moving_1 = 0; //0: not moving, 1: moving right, -1: moving left
unsigned long x_time = 0.0; //last time x was calculated

void loop() {
  unsigned long current_time = millis();
  unsigned long deltax = 0.0;

  if(moving_1 == -1){
    deltax = (current_time-x_time)/time_to_cross*-100;
    pos_1 = pos_1-deltax;
    if(pos_1 < 10){
      servo1.write(90);
    }else{
      servo1.write(95);
    }
    x_time = current_time;
  }else if(moving_1 == 1){
    deltax = (current_time-x_time)/time_to_cross*100;
    pos_1 = pos_1-deltax;
    if(pos_1 > 90){
      servo1.write(90);
    }else{
      servo1.write(85);
    }
    x_time = current_time;
  }else{
    servo1.write(90);
  }
  
  byte directionArray[2];
  if (Serial.available()) { Serial.readBytes(directionArray, 2); }
  if (directionArray[0] == 1) { 
    if(moving_1 != -1){
      moving_1 = -1; 
      x_time = current_time;
    }
  } else if (directionArray[1] == 1) { 
    if(moving_1 != 1){
      moving_1 = 1;
      x_time = current_time;
    }
  } else { 
    moving_1 = 0;
  }

  
}
