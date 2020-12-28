////////////////////////////////////
//
// Testing microphone on input A7
// Sending raw and processed read through serial port
// and mapand map to the led matrix
//

//LIBRARIES
#include <FastLED.h>

//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS  5 //Adjust the brightness of the led matrix

//VARIABLES
CRGB leds[NUM_LEDS];
float vol = 0;
const uint16_t samples = 128;

//SETUP
void setup() { 
  Serial.begin(9600);           //  setup serial
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness( BRIGHTNESS );
    
}


//LOOP
void loop() {
  float nvol = 0;
  
  //128 sample capture
  for (int i = 0; i<samples; i++){
      auto analog = analogRead(A7);
      auto micline = abs(analog-512);
      nvol = max(micline, nvol);
  }
  
  vol = (nvol + 1.0*vol)/2.0;
  //mapping of values ​​for led matrix
  int vol_leds = map(vol, 200, 512, 0, NUM_LEDS);
  if(vol>200){
    //we send microphone processed by serial port
    Serial.print(vol);
    Serial.print(" ");
    Serial.println(vol_leds);
  
    //before we turn off the LEDs
//-------------------------------    
    //PRUEBEN A PONER i<=NUM_LEDS
    //¿QUE SUCEDE?
//-------------------------------
    for (int i=0;i<NUM_LEDS;i++)
      leds[i] = CRGB::Black;
  
    //we turn on the mapped LEDs      
    //PRUEBEN A PONER i<=vol_leds
    //¿QUE SUCEDE?
    for (int i = 0; i < vol_leds; i++)
      leds[i] = CRGB::Green;
    
  } else {
    //Turn off the LEDs
    for (int j=0;j<=NUM_LEDS;j++) {
          leds[j] = CRGB::Black;}}
  
  FastLED.show();
}
