#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NeoPixelBus.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
const int redPin = 12;
const int greenPin = 15;
const int bluePin = 13;
char auth[] = "ba482bd93294469a827f8f663d3f6401";
char ssid[] = "New Curiosity Gym"; 
char pass[] = "CuriosityTwinkle"; 
int red=0;
int green=0;
int blue=0;

//int i = 0;
//int decColour=0; 
int rainbow=0;
boolean rainbowOff = false;
BlynkTimer timer;
void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
 }

BLYNK_WRITE(V1)
{ 

  rainbow = param.asInt(); // assigning incoming value from pin V1 to a variable
 
}

void rainbowFade()
    {
      if(rainbow == 1)
      {
    unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(10);
    }
  }
   rainbowOff = true;
    } 

  if(rainbow == 0 && rainbowOff == true)
    {
       setColourRgb(0,0,0);
      rainbowOff = false;
    }
    }
BLYNK_WRITE(V4)
{ 
   
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
  setColourRgb(red, green,blue);
      
 }   


void setup() {
   // Start off with the LED off.
  setColourRgb(255,0,0);
  delay(1000);
  setColourRgb(0,0,0);
    WiFiManager wifiManager;
   //wifiManager.resetSettings();
     wifiManager.autoConnect("CG Mood Lamp");
    // Serial.println("connected...yeey :)");
    /// Serial.println( WiFi.SSID().c_str());
     //Serial.println(WiFi.psk().c_str());
    //String ssid1 = WiFi.SSID().c_str();
    wifiManager.setConfigPortalTimeout(180);  // after 3 minutes try to autoconnect again
    setColourRgb(0,255,0);
    delay(1000);
    setColourRgb(0,0,0);
 // Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  while (Blynk.connect() == false) {}
  //ArduinoOTA.setHostname("xxxxxxx"); // OPTIONAL
 timer.setInterval(8000L, rainbowFade);
  ArduinoOTA.begin();
  
}



void loop() {
  Blynk.run();
   timer.run();
  ArduinoOTA.handle();
}
