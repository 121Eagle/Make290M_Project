#include<Wire.h>
#include<ESP32Servo.h>
#include<VL53L0X.h>
const byte endpoint = 50;
const byte Speed = 45;
byte directionV = 1;

VL53L0X sensor;
Servo servo1;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  servo1.attach(23);
  servo1.write(90);
  // put your setup code here, to run once:
  if(!sensor.init()){
    Serial.println("Failed to detect and initialize");
    while(1){}
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = sensor.readRangeSingleMillimeters();
  if(x < endpoint){
      Serial.print("TOO CLOSE!:   ");
      Serial.print("Distance is: ");
      Serial.println(x,DEC);
      directionV *=-1;
    }
    servo1.write(int(90-(directionV * Speed)));
    
}
