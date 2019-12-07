#include<Wire.h>
#include<VL53L0X.h>
#include <Adafruit_SSD1306.h>
#include<ESP32Servo.h>
#define BUTTON0 34
#define BUTTON1 0
#define BUTTON2 35

Adafruit_SSD1306 lcd(128, 64);
Servo servo1;
Servo servo2;
VL53L0X sensor;

const byte minDistance = 50;

void setup() {
  //Set buttons
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);    

  //Set servos
  servo1.attach(23);
  servo2.attach(18);
  
  // Start LCD scren
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.setTextColor(WHITE);
  lcd.clearDisplay();
  lcd.display(); 

  //Start serial
  Serial.begin(9600);

  //Set sensor
  if(! sensor.init()){
    Serial.println("ERRROR!!! sensor not initialized");
    while(true){}
  }
}

int playing = 0; //Whether or not the game has started
unsigned long last_flip[] = {0.0, 0.0, 0.0}; //Last time button was flipped
byte prev_state[] = {1, 1, 1}; //Last known state of buttons

/* A menu for holding the game until a button is pressed.*/
void menu(){
  while(!playing){
    //Check button for start
    unsigned long current_time = millis();
    byte curr_state = digitalRead(BUTTON0); 
    if((curr_state==0 && prev_state[0]==1) && (current_time-last_flip[0]>=100)){ 
      playing = 1;
      last_flip[0] = current_time;
    }
    prev_state[0] = curr_state;

    //Write menu
    lcd.clearDisplay();
    lcd.setCursor(0,0);
    lcd.print("PRESS BUTTON 0");
    lcd.display();
  }
}

int can_move1 = 1; //Can servo 1 move?
int can_move2 = 1; //Can servo 2 move? 

byte canServoMove(int can_move){ //why not a boolean?
  boolean can;
  if(sensor.readRangeSingleMillimeters() < minDistance){
    can = false;
  }else{
    can = true;
  }
  if(!can){//servo cannot move, therefore, return -1;
    return -1;
  }else if(can_move == 1 || can_move == 0){//servo can move, and servo already can continue, therfore carryon
    return 0;
  }else{//start moving
    return 1;
  }
}

void game(){
  randomSeed(analogRead(A13)); // sample analog pin as random seed
  int seconds = random(10, 20); //pick random time between 10 and 15 seconds
  while(playing){
    //draw lcd
    lcd.clearDisplay();
    lcd.setCursor(0,0);
    lcd.print("PLAYING...");
    lcd.setCursor(0,8);
    lcd.print(String(seconds));
    lcd.display();

    //Check whether servos can move
    can_move1 = canServoMove(can_move1);
    //TODO: Rotate sensor 180 degrees, to see if other servo can move
    can_move2 = canServoMove(can_move2);
    //increment time
    delay(1000);
    seconds--;
    if(seconds <= 0){
      playing = 0;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  menu();
  game();
}
