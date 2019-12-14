#include <Adafruit_SSD1306.h>
#include<ESP32Servo.h>
#include<Wire.h>
#include<VL53L0X.h>

#define BUTTON0 34
#define BUTTON1 0
#define BUTTON2 3

#ifndef UP_BUTTON
#define UP_BUTTON BUTTON0
#endif
const byte up = UP_BUTTON;
#ifndef DOWN_BUTTON
#define DOWN_BUTTON BUTTON2
#endif
const byte down = DOWN_BUTTON;

Adafruit_SSD1306 lcd(128, 64);
Servo servo1, servo2;
VL53L0X sensor;

void setup() {
  //Set buttons
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);    

  //Set servos
  servo1.attach(23);
  servo2.attach(18);
  servo1.write(90), servo2.write(90);

  //Start IR sensors
  sensor.init();
  sensor.startContinuous();
  
  // Start LCD scren
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.setTextColor(WHITE);
  lcd.clearDisplay();
  lcd.display(); 

  //Start serial
  Serial.begin(230400);
}

int playing = 0; //Whether or not the game has started. Also indicates difficulty (1-3)
byte processing_input[3];
volatile byte movement = 0;

void IRAM_ATTR moveUp(){
  movement = 1-movement;
}

void IRAM_ATTR moveDown(){
  movement = -1 - movement;
}

/* A menu for holding the game until a button is pressed.*/
void menu(){
  //Write menu
  lcd.clearDisplay();
  lcd.setCursor(0,0);
  lcd.print("SELECT DIFFICULTY");
  lcd.setCursor(0,8);
  lcd.print("1 2 3");
  lcd.setCursor(0,16);
  lcd.print("(Use keyboard)");
  lcd.display();
  
  while(playing == 0){
    attachInterrupt(digitalPinToInterrupt(up), moveUp, CHANGE);
    attachInterrupt(digitalPinToInterrupt(down), moveDown, CHANGE);
    //Check input for start. Use number for difficulty
    if(Serial.available()){ Serial.readBytes(processing_input,3); }
    if(processing_input[0]){
      playing = 1;
    }else if(processing_input[1]){
      playing = 2;
    }else if(processing_input[2]){
      playing = 3;
    }
    //Make sure servos are stopped
    servo1.write(90), servo2.write(90);
  }
  detachInterrupt(digitalPinToInterrupt(up));
  detachInterrupt(digitalPinToInterrupt(down));
}

/* Checks the IR sensor and returns
  0: if the servo can move
  1: if the servo is too far
  -1: if the servo is too close
*/

const int maxDistance = 220; //farthest the servo can get
const int minDistance = 50; //closest the servo can get

int canMove(){
    //Check whether top servo can move
    int range = sensor.readRangeContinuousMillimeters();
    if(range < minDistance){
      return -1;
    }else if (range > maxDistance){
      return 1;
    }else{
      return 0;
    }
}

/* Runs the game */
void game(){
  randomSeed(analogRead(A13)); // sample analog pin as random seed
  int seconds = random(10, 20); //pick random time between 10 and 15 seconds
  unsigned long current_time = millis(); //current time in milliseconds
  unsigned long end_time = current_time+seconds*1000; //Set end time

  int can_move1 = 1; //Can servo 1 move?
  int can_move2 = 1; //Can servo 2 move? 
  int auto_speed = 10; //Holds the current speed of servo
  unsigned long last_flip = current_time; //Holds the last time the auto motor was flipped

  //game loop
  while(playing != 0){
    current_time = millis(); //Set time

    //Check for end of game
    if(current_time >= end_time){
      
      //Write to electromagnet to drop the ball

      delay(3000);
      
      //Check whether or not it landed on the hoop
      //Display results
      //Reset electromagnet
      
      playing = 0;
    }

    //Check if servo1 can move
    can_move1 = canMove();
    if(can_move1 == 1){ 
      //If too far, write CW
      servo1.write(80);
    }else if(can_move1 == -1){
      //If too close, write CCW
      servo1.write(100);
    }else{
      servo1.write(100);
    }
    

    //Check input
    if(Serial.available()){ Serial.readBytes(processing_input,3); }
    
    //Check whether bottom servo can move in the inputted direction
    /**Ignoring this code for right now
    *can_move2 = 1; //TODO: input IR sensor function
    
    if(processing_input[0] && can_move1 != 1){
      servo2.write(100);
    }else if(processing_input[2] && can_move1 != -1){
      servo2.write(80);
    }else{
      servo2.write(90);
    }
    */
    servo2.write(90 + (45 * movement));
  
    //draw lcd
    lcd.clearDisplay();
    lcd.setCursor(0,0);
    lcd.print("PLAYING...");
    lcd.setCursor(0,8);
    lcd.print(String((end_time-current_time)/1000));
    lcd.display();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  menu();
  game();
}
