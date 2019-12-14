#include <Adafruit_SSD1306.h>
#include<ESP32Servo.h>
#include<Wire.h>
#include<VL53L0X.h>

#define BUTTON0 34
#define BUTTON1 0
#define BUTTON2 35

Adafruit_SSD1306 lcd(128, 64);
Servo servo1, servo2;
VL53L0X sensor;

void setup() {
  //Start serial
  Serial.begin(230400);
  Wire.begin();
  Serial.println("*** SETUP CLAW GAME ***");
  
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
}

int playing = 0; //Whether or not the game has started. Also indicates difficulty (1-3)

unsigned long last_flip[] = {0.0, 0.0, 0.0};
byte prev_state[] = {1, 1, 1};

/* A menu for holding the game until a button is pressed.*/
void menu(){
  unsigned long current_time = millis();
  playing = 0;
  
  //Make sure servos are stopped
  servo1.write(90), servo2.write(90);
  
  //Write menu
  lcd.clearDisplay();
  lcd.setCursor(0,0);
  lcd.print("SELECT DIFFICULTY");
  lcd.setCursor(0,8);
  lcd.print("BUTTON0: EASY");
  lcd.setCursor(0,16);
  lcd.print("BUTTON1: NORMAL");
  lcd.setCursor(0,24);
  lcd.print("BUTTON2: HARD");
  lcd.display();
  
  Serial.println("*** BEGIN MENU ***");
  
  while(playing <= 0){
    current_time = millis();
    
    //Check input for start. Use number for difficulty\

    //button 0 - reverse speed
    byte curr_state = digitalRead(BUTTON0); 
    if((curr_state==0 && prev_state[0]==1) && (current_time-last_flip[0]>=100)){ 
      Serial.println("BUTTON 0");
      playing = 1;
      last_flip[0] = current_time;
    }
    prev_state[0] = curr_state;
  
    //button 1 - pause/resume
    curr_state = digitalRead(BUTTON1); 
    if((curr_state==0 && prev_state[1]==1) && (current_time-last_flip[1]>=100)){ 
      Serial.println("BUTTON 1");
      playing = 2;
      last_flip[1] = current_time;
    }
    prev_state[1] = curr_state;
  
    //button 2 - halve frame delay
    curr_state = digitalRead(BUTTON2); 
    if((curr_state==0 && prev_state[2]==1) && (current_time-last_flip[2]>=100)){ 
      Serial.println("BUTTON 2"); 
      playing = 3;
      last_flip[2] = current_time;
    }
    prev_state[2] = curr_state;
  }
} 

/* Runs the game */
void game(){
  randomSeed(analogRead(A13)); // sample analog pin as random seed
  int seconds = 25; //pick random time between 10 and 15 seconds
  unsigned long current_time = millis(); //current time in milliseconds
  unsigned long end_time = current_time+seconds*1000; //Set end time
  unsigned long change_direction = current_time; //how long until 
  int auto_speed = playing*5+5; //Holds the current speed of servo
  int man_speed = 90; //speed of manually controlled servo
  
  Serial.println("*** BEGIN CLAW GAME ***");
  
  //game loop
  while(playing != 0){
    current_time = millis(); //Set time
    
    //Check for end of game
    if(current_time >= end_time){
      Serial.println("*** END OF CLAW GAME ***");
      
      //Stop servos
      servo1.write(90);
      servo2.write(90);
      
      //draw lcd
      lcd.clearDisplay();
      lcd.setCursor(0,0);
      lcd.print("GAME OVER");
      
      //Check sensor and determine whether you were lined up or not
      lcd.setCursor(0,8);
      if(sensor.readRangeSingleMillimeters() > 100){
        lcd.print("TOO FAR");
      }else{
        lcd.print("YOU WIN");
      }
      lcd.display();
      
      delay(3000);
      
      //Check whether or not it landed on the hoop
      //Display results
      //Reset electromagnet
      
      playing = 0;
    }else{
      //draw lcd
      lcd.clearDisplay();
      lcd.setCursor(0,0);
      lcd.print("PLAYING...");
      lcd.setCursor(0,8);
      lcd.print(String((end_time-current_time)/1000));
      lcd.display();

      //Write servo 1
      if(current_time >= change_direction){
        Serial.println("CHANGING DIRECTION");
        Serial.println(String(90+auto_speed));
        servo1.write(90+auto_speed);
        auto_speed *= -1;
        change_direction += random(3000+(3000-playing*1000), 5000+(3000-playing*1000));
      }
      
      //Check input
      //button 0
      byte curr_state = digitalRead(BUTTON0); 
      if((curr_state==0 && prev_state[0]==1) && (current_time-last_flip[0]>=100)){
        Serial.println("BUTTON0");
        Serial.println(String(man_speed));
        if(man_speed < 110) man_speed += 20;
        last_flip[0] = current_time;
      }
      prev_state[0] = curr_state;

      //button 1
      curr_state = digitalRead(BUTTON1); 
      if((curr_state==0 && prev_state[1]==1) && (current_time-last_flip[1]>=100)){ 
        Serial.println("BUTTON 1");
        end_time = 0.0;
        last_flip[1] = current_time;
      }
      prev_state[1] = curr_state;

      //button 2
      curr_state = digitalRead(BUTTON2); 
      if((curr_state==0 && prev_state[2]==1) && (current_time-last_flip[2]>=100)){ 
        Serial.print("BUTTON 2");
        Serial.println(String(man_speed));
        if(man_speed > 70) man_speed -= 20;
        last_flip[2] = current_time;
      }
      prev_state[2] = curr_state;

      servo2.write(man_speed);
    }//end else
  }//end while
}//end function

void loop() {
  // put your main code here, to run repeatedly:
  menu();
  game();
}
