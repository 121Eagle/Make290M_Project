#include<Wire.h>
#include<VL53L0X.h>
VL53L0X sensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  Wire.begin();
  sensor.init();
  sensor.startContinuous();
  Serial.println("***START TEST****"); 
}

const int maxDistance = 300; //farthest the servo can get
const int minDistance = 50; //closest the servo can get

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(String(sensor.readRangeContinuousMillimeters())); 
  int range = sensor.readRangeContinuousMillimeters();
  if(range < minDistance){
    Serial.println(" Too Close"); 
  }else if (range > maxDistance){
    Serial.println(" Too Far"); 
  }else{
    Serial.println("");
  }
}
