#include <FastLED.h>
//FIXED GLOBAL VARS
#define DATA_PIN 8  //Define led data pin in
#define LED_TYPE NEOPIXEL  //define type of led
#define NUM_LEDS 64  //num of leds in strip

struct CRGB leds[NUM_LEDS];  // Initialize LED array


void setup() {
  // put your setup code here, to run once:
  delay(2000); //for safe startup
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( 5 );
}

void loop() {
leds[0] = CRGB::Blue;
leds[7] = CRGB::Green;
leds[56]= CRGB::Orange;
leds[63]= CRGB::Red;
FastLED.show();       //start the leds
delay(50);
}
