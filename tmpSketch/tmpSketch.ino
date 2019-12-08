const int Delay = 1000;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  Serial.println("***TEST START***");
}
unsigned long timeout=Delay;
void loop() {
  // put your main code here, to run repeatedly:
  if(millis() > timeout){
    digitalWrite(2,1-digitalRead(2));
    Serial.println("flipped");
    timeout = millis() + Delay;
  }
}
