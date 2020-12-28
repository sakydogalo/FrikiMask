////////////////////////////////////
//Test mouths with the microphone
//

//LIBRARIES
#include <FastLED.h>

//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS 255 //Adjust the brightness of the led matrix
#define FIL 8
#define COL 8

//GLOBAL VARIABLES

//first mouth
CRGB boca_0[FIL][COL] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

//second mouth
CRGB boca_1[FIL][COL] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

//third mouth
CRGB boca_2[FIL][COL] = {
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1}, 
    {0,1,0,0,0,0,1,0},
    {0,0,1,0,0,1,0,0},
    {0,0,0,1,1,0,0,0},
};

//working LEDs
CRGB leds[NUM_LEDS];
float vol = 0;
const uint16_t samples = 128;

//FUNCTIONS
void copia_matriz( const CRGB matriz[][ COL ] ) {
  CRGB res[NUM_LEDS];
  int k=0;
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      if ( (x % 2) == 0) {
        leds[k]=matriz[x][y];
        } else {
          leds[k]=matriz[x][7-y];
          }
      k=k+1;
      }}
}

//SETUP
void setup() { 
  Serial.begin(9600);                                       //  setup serial
  // Config matrix comunication with driver WS2812B
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  
  FastLED.setBrightness( BRIGHTNESS );                      //Adjust Brightness
}

///LOOP
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
    if (vol_leds<25) copia_matriz(boca_0);  //mouth 0
    if (vol_leds>=25) copia_matriz(boca_1); //mouth 1
    if (vol_leds>40) copia_matriz(boca_2);  //mouth 2
    //Serial.print(vol);
    //Serial.print(" ");
    //Serial.println(vol_leds);
    } 
  else
      //Turn off the LEDs
      for (int j=0;j<NUM_LEDS;j++)
        leds[j] = CRGB::Black;
  FastLED.show();
  delay(150);
}
