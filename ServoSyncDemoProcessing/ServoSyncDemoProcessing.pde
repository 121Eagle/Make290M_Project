import processing.serial.*;

final int fps = 30;
final int BAUD_RATE = 230400;

Serial port;
//final String serialName = "COM4";
final String serialName = "/dev/ttyUSB0";

byte[] data = new byte[2];
boolean keys[] = new boolean[2];  // key statuses
final int D=90;  // circle diameter

void setup() {
  size(190, 100);
  background(255);  
  frameRate(fps);  
  
  port = new Serial(this, serialName, BAUD_RATE);
}

void draw(){
  background(255);  // clear background with white
  for(int i=0;i<2;i++) {
    fill(keys[i]?0:255);
    ellipse(i*100+D/2, 50, D, D);
  }
}

void keyPressed() {
  if(key>='1'&&key<='2') {
    int i=key-'1';
    keys[i]=true;
  }
  byte write_me[] = {byte(keys[0]), byte(keys[1])};
  port.write(write_me);
}

void keyReleased() {
  if(key>='1'&&key<='2') keys[key-'1']=false;
  byte write_me[] = {byte(keys[0]), byte(keys[1])};
  port.write(write_me);
}
