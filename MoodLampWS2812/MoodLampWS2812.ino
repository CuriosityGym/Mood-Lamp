#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NeoPixelBus.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <TimeLib.h>
BlynkTimer timer;
const uint16_t PixelCount = 21; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 4;  // make sure to set this to the correct pin, ignored for Esp8266
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
int red;
int blue;
int green;
char auth[] = "ba482bd93294469a827f8f663d3f6401";
char ssid[] = "New Curiosity Gym"; 
char pass[] = "CuriosityTwinkle"; 
typedef struct{
  int r;
  int g;
  int b;
}rgb;

boolean rainbowOff = false;
int rainbow=0;
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
rgb Wheel(byte WheelPos) {
  //int a,b,c;
  rgb rgb1;
  if(WheelPos < 85) {
     rgb1 = {WheelPos * 3, 255 - WheelPos * 3, 0};
    return rgb1;
   //return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   rgb1 ={255 - WheelPos * 3,0, WheelPos * 3};
    return rgb1;
  // return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   rgb1={0, WheelPos * 3, 255 - WheelPos * 3};
    return rgb1;
//   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

BLYNK_WRITE(V1)
{ 
  
   rainbow = param.asInt(); // assigning incoming value from pin V1 to a variable
 
}
void rainbowFade()
    {
      strip.Begin();
      strip.Show();
      if(rainbow == 1)
        {
          uint16_t i, j;
      
               for(j=0; j<256; j++)
                  {
                   for(i=0; i<PixelCount; i++) 
                      {
                        rgb rgb1 = Wheel((i+j) & 255);
                        strip.SetPixelColor(i, RgbColor(rgb1.r, rgb1.g, rgb1.b));
                      }
                   strip.Show();
                   delay(20);
                  }
             rainbowOff = true;
             
        }      

     if(rainbow == 0 && rainbowOff == true)
       {
        for (int i=0; i < PixelCount; i++) {
        strip.SetPixelColor(i,RgbColor(0,0,0));}
        strip.Show();
        rainbowOff = false;
       }
    }
BLYNK_WRITE(V4)
{ 
   strip.Begin();
    strip.Show();
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
  for (int i = 0; i < PixelCount; i++)
      {
        strip.SetPixelColor(i,RgbColor(red,green,blue));
       }
        strip.Show();
      
 }   


void setup() {
  strip.Begin();
    strip.Show();
    for (int i=0; i < PixelCount; i++) {
        strip.SetPixelColor(i,RgbColor(255,0,0));}
        strip.Show();
        delay(1000);
    for (int i=0; i < PixelCount; i++) {
        strip.SetPixelColor(i,RgbColor(0,0,0));}
        strip.Show();
    WiFiManager wifiManager;
  // wifiManager.resetSettings();
     wifiManager.autoConnect("CG Mood Lamp");
    // Serial.println("connected...yeey :)");
    /// Serial.println( WiFi.SSID().c_str());
     //Serial.println(WiFi.psk().c_str());
    //String ssid1 = WiFi.SSID().c_str();
    wifiManager.setConfigPortalTimeout(180);  // after 3 minutes try to autoconnect again
    for (int i=0; i < PixelCount; i++) {
        strip.SetPixelColor(i,RgbColor(0,255,0));}
        strip.Show();
        delay(1000);
    for (int i=0; i < PixelCount; i++) {
        strip.SetPixelColor(i,RgbColor(0,0,0));}
        strip.Show();
 // Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  while (Blynk.connect() == false) {}
  //ArduinoOTA.setHostname("xxxxxxx"); // OPTIONAL
   timer.setInterval(6500L, rainbowFade);
  ArduinoOTA.begin();
}



void loop() {
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();
}
