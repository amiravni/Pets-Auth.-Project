
#include <Stepper.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(68, 13, NEO_GRB + NEO_KHZ800);



const char* ssid = "";
const char* password = "";
WiFiServer server(80);
const int stepsPerRevolution = 800;  
Stepper myStepper(stepsPerRevolution, 14, 5, 4, 12);

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(15);
  // initialize the serial port:
  Serial.begin(115200);
    delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP()); 
  
    strip.begin();
  strip.show(); // Initialize all pixels to 'off'   
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void loop() {

if (WiFi.status() != WL_CONNECTED) {
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}


  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }  

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();





// Match the request
  int val=-1;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }


// Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  if (val==1) {
  colorWipe(strip.Color(0, 255, 0), 25);
  colorWipe(strip.Color(0, 0, 0), 15);
  colorWipe(strip.Color(0, 255, 0), 25);
  colorWipe(strip.Color(0, 0, 0), 15);
  for(int i;i<10;i++) {
  myStepper.step(stepsPerRevolution/10);
  delay(0);
  }
  for(int i;i<10;i++) {
  myStepper.step(-stepsPerRevolution/10);
  delay(0);
  }
  }

  if (val==0) {
    colorWipe(strip.Color(255, 0, 0), 35);
    colorWipe(strip.Color(0, 0, 0), 15);
    colorWipe(strip.Color(255, 0, 0), 25);
     colorWipe(strip.Color(0, 0, 0), 15);
    colorWipe(strip.Color(255, 0, 0), 15);
    colorWipe(strip.Color(0, 0, 0), 15);
}

