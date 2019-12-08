#include<Wire.h>
#include<VL53L0X.h>
#include<ESP32Servo.h>

VL53L0X sensor;
Servo servo1;

void setup() {
  Serial.begin(230400);
  Wire.begin();
  
  sensor.init();
  //sensor.setTimeout(500);

  servo1.attach(23);
}

const int maxDistance = 220; //farthest the servo can get
const int minDistance = 50; //closest the servo can get

int canMove(){
    //Check whether top servo can move
    int range = sensor.readRangeSingleMillimeters();
    Serial.print(String(range));
    if(range < minDistance){
      return -1;
    }else if (range > maxDistance){
      return 1;
    }else{
      return 0;
    }
}

void loop() {
  //Check if servo1 can move
    int can_move1 = canMove();
    if(can_move1 == 1){ 
      //If too far, write CW
      servo1.write(95);
      Serial.println(" TOO FAR");
    }else if(can_move1 == -1){
      //If too close, write CCW
      servo1.write(85);
      Serial.println(" TOO CLOSE");
    }else{
      servo1.write(90);
      Serial.println("");
    }
}
