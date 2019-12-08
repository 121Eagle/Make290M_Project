#include<Wire.h>
#include<VL53L0X.h>
VL53L0X sensor;
const byte minDistance = 100;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  Wire.begin();
  sensor.init();
  sensor.startContinuous();
}
int can_move2 = 1;
int can_move1 = 1;
void loop() {
  // put your main code here, to run repeatedly:
  if(sensor.readRangeContinuousMillimeters() < minDistance){
   Serial.println("Too Close"); 
  }
}
