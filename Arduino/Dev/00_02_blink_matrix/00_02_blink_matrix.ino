/////////////////////////////////////
//
// Simple program that blink first led
//

//LIBRARIES
#include <FastLED.h>

//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS  5  //Adjust the brightness of the led matrix

//VARIABLES
CRGB leds[NUM_LEDS];

//SETUP
void setup() { 
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness( BRIGHTNESS );
}

//LOOP
void loop() { 
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
}
