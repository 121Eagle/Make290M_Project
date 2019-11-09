#include<ESP32Servo.h>
Servo servo1,servo2;
void setup() {
  // put your setup code here, to run once:
  servo1.attach(23), servo2.attach(18);
}

void moveServo(int value){
  servo1.rotate(value);
  servo2.rotate(value);
}

void loop() {
  // put your main code here, to run repeatedly:

}
