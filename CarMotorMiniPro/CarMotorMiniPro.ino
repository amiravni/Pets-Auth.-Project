
#define MYID 5
#define DEBUG 0
#define GAPRML 0
#define GAPRMLSTOP 0

#define MTRLFTPIN1 2
#define MTRLFTPIN2 5
#define MTRGHTPIN1 3
#define MTRGHTPIN2 4
#define ENABLEMOTORL 9
#define ENABLEMOTORR 10
#define LEDSPIN 6

#define MSGLEN 5
#define DT2STOP 100


#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, LEDSPIN, NEO_GRB + NEO_KHZ800);
unsigned long TimeNoComm = 0;
byte left2RightDiff=GAPRML;
byte left2RightStopDiff = GAPRMLSTOP;
// —————————————————————————  Motors
int motor_left[] = {  MTRLFTPIN1, MTRLFTPIN2};
int motor_right[] = {  MTRGHTPIN1, MTRGHTPIN2};

// ————————————————————————— Setup
void setup() {
 if (DEBUG) Serial.begin(9600);

  // Setup motors
  int i;
  for(i = 0; i < 2; i++){
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }
  pinMode(ENABLEMOTORL,OUTPUT);
  pinMode(ENABLEMOTORR,OUTPUT);
  
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();   
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = MSGLEN*sizeof(byte);       
  Mirf.config();

TimeNoComm = millis();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
/*
TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(CS12);
  OCR1A = 0;
  OCR1B = 0;*/
  
}

void loop() {

  byte data[Mirf.payload];
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData(data);
 if (DEBUG)  Serial.println(":");
   if (DEBUG) { for (int i=0; i<MSGLEN;i++)   Serial.println( (int)data[i] ); }

 if (data[0] ==MYID ||  data[0] ==0 ) {

   switch (data[1])
   {
     case 0: // INIT
      left2RightDiff=data[2];
      left2RightStopDiff = data[3];
     break;
     case 1: // FW
//     drive_forward();
      drive_backward();
     break;     
     case 2: // BW
 //    drive_backward();
     drive_forward();
     break;   
     case 3: // Left
//     turn_left();
     turn_right();
     break;     
     case 4: // Right
//     turn_right();
     turn_left();
     break;  
     case 5: // Stop  +  Stop procedure
     motor_stop();
     break;  
     case 6: // All LEDs one Color
    colorWipe(strip.Color(data[2], data[3], data[4]), 0);
     break;
     case 7: // All LEDs one Color
  colorWipe(strip.Color(data[2], data[3], data[4]), 100);
  //  colorWipe(strip.Color(255, 0, 255), 100);
     break;                 
     default:
     if (data[1] > 100)
     {
      strip.setPixelColor(data[1] - 100, strip.Color(data[2], data[3], data[4]));
      strip.show();
     }
   }
   
   if (data[1] >=1 && data[1] <=4 ) {
     if (left2RightDiff < 128)
     {
     analogWrite(ENABLEMOTORL, data[2] - left2RightDiff );
     analogWrite(ENABLEMOTORR, data[3]  );  
     }
     else
     {
     analogWrite(ENABLEMOTORL, data[2]  );
     analogWrite(ENABLEMOTORR, data[3] - (255 - left2RightDiff)  );         
     }
     TimeNoComm = millis();
   }
   
 }
 
 }

if ( (millis() - TimeNoComm) > DT2STOP)
{
      analogWrite(ENABLEMOTORL, 0);
      analogWrite(ENABLEMOTORR, 0);
      motor_stop(); 
}


}


void motor_stop(){
  if (left2RightStopDiff < 128)
  {
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  delay(left2RightStopDiff);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
  }
  else
  {
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);  
  delay(255 - left2RightStopDiff);  
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);

  }

}

void drive_forward(){
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_left[1], LOW);

  digitalWrite(motor_right[0], HIGH);
  digitalWrite(motor_right[1], LOW);
}

void drive_backward(){
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);

  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
}

void turn_left(){
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);

  digitalWrite(motor_right[0], HIGH);
  digitalWrite(motor_right[1], LOW);
}

void turn_right(){
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_left[1], LOW);

  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
