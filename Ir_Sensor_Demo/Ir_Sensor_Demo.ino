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
}

void loop() {
  Serial.println(String(sensor.readRangeSingleMillimeters()));
}
