////////////////////////////////////
//
// Full matrix blink tree colors (RGB) for 4 matrices 8X8
// 

//LIBRARIES
#include <FastLED.h>


//CONSTANTS
#define NUM_LEDS 256   //Number of LEDs for 4 matrices 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS  25 //Adjust the brightness of the led matrix

//VARIABLES
CRGB leds[NUM_LEDS];

//SETUP
void setup() { 
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness( BRIGHTNESS );
}

//LOOP
void loop() {
  
  //To Green
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  delay(500);
  
  //To Red
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
  delay(500);
  
  //To Yellow
  for (int i = 0; i <= NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
    }
    FastLED.show();
    delay(500);

  //To Black
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
  
}
