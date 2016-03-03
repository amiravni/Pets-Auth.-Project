
 /*
  if(keyPressed) {
       port.write (key);
}
*/
import processing.serial.*;

Serial port;
String portname = "COM3";  
int baudrate = 9600;
int value = 0;  
 
float bx;
float by;
int boxSize = 20;
boolean overBox = false;
boolean locked = false;
boolean pinFlag = false;
float xOffset = 0.0; 
float yOffset = 0.0; 
float radius = 0.0;
final char maxSpeed=254;
int car_id = 4;
void setup() 
{
    port = new Serial(this, portname, baudrate);
  println(port);
  size(480, 640);
  bx = width/2.0;
  by = height/2.0;
  rectMode(RADIUS);  
  ellipseMode(RADIUS);
}

void draw() 
{ 
  background(0);
  if(locked) { 

    char xDir = 2;
  char yDir = 2;
  char xPower = 255;
  char yPower = 255;
  char r = 0;
  char g = 0;
  char b = 0;
  boolean rgb_func = false;
    switch (key) {
     case 'w':
       if (car_id == 4){
         xDir = 0;
         yDir = 0; 
       } else {
         xDir = 1;
         yDir = 1;
       }
        break;
     case 'a':
      if (car_id == 5){
         xDir = 0;
         yDir = 1; 
       } else {
         xDir = 1;
         yDir = 0;
       }  
      xPower = 100;
      yPower = 100;
      break;
     case 'd':
      if (car_id == 4){
         xDir = 0;
         yDir = 1; 
       } else {
         xDir = 1;
         yDir = 0;
       }
      xPower = 100;
      yPower = 100;
      break;
     case 's':
       if (car_id == 5){
         xDir = 0;
         yDir = 0; 
       } else {
         xDir = 1;
         yDir = 1;
       }
     break;
      case '4':
        println("4");
        if (car_id == 4){
          rgb_func = true;
          r = 255;
          b = 0;
          g = 0;
        }
        car_id = 4;
        break;
      case '5':
      println("5");
        if (car_id == 5){
          rgb_func = true;
          r = 0;
          b = 0;
          g = 255;
        }
        car_id = 5;
        break;
        
       default: 
    locked=false;
    }
    /*
  // Test if the cursor is over the box 
  if (mouseX > bx-boxSize && mouseX < bx+boxSize && 
      mouseY > by-boxSize && mouseY < by+boxSize) {
    overBox = true;  
    if(!locked) { 
      stroke(255); 
      fill(153);
    } 
  } else {
    stroke(153);
    fill(153);
    overBox = false;
  }
   ellipse(width/2.0, height/2.0, 20, 20); 
  // Draw the box
  rect(bx, by, boxSize, boxSize);

float RELbx = (bx-width/2.0)*2;
float RELby = (by-height/2.0)*2;
float ROTbx = cos(3*PI/4)*RELbx - sin(3*PI/4)*RELby;
float ROTby = sin(3*PI/4)*RELbx + cos(3*PI/4)*RELby;
  char xPower = (char)min(maxSpeed,(char)abs(ROTbx));
  char yPower = (char)min(maxSpeed,(char)abs(ROTby));  
  char xDir = ((ROTbx) > 0 ) ? (char)1 : (char)0;
  char yDir = ((ROTby) > 0 ) ? (char)1 : (char)0;
  */

  if (((xPower>20 || yPower > 20) && locked && xDir < 2 && yDir < 2 ) || rgb_func) {
    
  port.write(255);
  port.write(254);
  port.write(100);
  port.write(car_id);
  if (!rgb_func){
  
    if (xDir==1 && yDir==1) {
      port.write(1);
    }
    else if (xDir==0 && yDir==0) {
      port.write(2);
    }
    else if (xDir==1 && yDir==0) {
      port.write(3);
    }
    else if (xDir==0 && yDir==1) {
      port.write(4);
    }
    
    port.write(xPower);
    port.write(yPower);
    //port.write(xPower);
    //port.write(xDir);
    //port.write(yPower);
    //port.write(yDir);  
   // port.write(pinFlag ? (char)1 : (char)0);  
   port.write(0);
  } else {
    port.write(6);
    port.write(r);
    port.write(g);
    port.write(b);
    rgb_func = false;
    println("RGB function");
  }
   while (port.available() > 0) {
    int inByte = port.read();
    println(inByte);
  }
  }
    println("X:" + (int)xPower + "(" + (int)xDir + ")" + " Y:" + (int)yPower + "(" + (int)yDir + ")"  );

  }
}

void mousePressed() {
  if(overBox) { 
    locked = true; 
    fill(255, 255, 255);
  } else {
    locked = false;
  }
  xOffset = mouseX-bx; 
  yOffset = mouseY-by; 
  
    if (mouseButton == LEFT)
  pinFlag=true;
  if (mouseButton == RIGHT)
  pinFlag=false;

}

void mouseDragged() {
  if(locked) {
    bx = mouseX-xOffset; 
    by = mouseY-yOffset; 
  }
}

void mouseReleased() {
  locked = false;
}

void keyPressed()
{
 locked = true; 
 // println((int)key);
}

void keyReleased()
{
 locked = false; 
}