////////////////////////////////////
//Test more mouths (five) with the microphone
//

//LIBRARIES
#include <FastLED.h>

//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS 20  //Adjust the brightness of the led matrix
#define FIL 8
#define COL 8
#define COULEUR CRGB::Blue     //The selected color to paint

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
    {0,0,0,1,1,0,0,0},
    {1,1,1,0,0,1,1,1},
    {1,1,1,0,0,1,1,1},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

//thirt mouth
CRGB boca_2[FIL][COL] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

//fourth mouth
CRGB boca_3[FIL][COL] = {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0}
};

//fifth mouth
CRGB boca_4[FIL][COL] = {
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
};


//working LEDs
CRGB leds[NUM_LEDS];
float vol = 0;
const uint16_t samples = 128;

//FUNCTIONS
void copia_matriz( const CRGB matriz[][ COL ] ) {
  //snake-shaped matrix (zig-zag)
  int k=0;
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      if ( (x % 2) == 0) {
        if(matriz[x][y]) leds[k]= COULEUR;      //---->
        else leds[k]= 0;
        } else {
          if(matriz[x][7-y]) leds[k]= COULEUR;  //<----
          else leds[k]= 0;
          }
      k=k+1;
      }}
}

void setup() {
    Serial.begin(9600);                                       //  setup serial
    // Config matrix comunication with driver WS2812B
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  
    FastLED.setBrightness( BRIGHTNESS );                      //Adjust Brightness
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
    Serial.println(vol_leds);
    if (vol_leds<15)  copia_matriz(boca_0);//mouth 0 
    if (vol_leds>=15) copia_matriz(boca_1);//mouth 1
    if (vol_leds>25)  copia_matriz(boca_2);//mouth 2
    if (vol_leds>35)  copia_matriz(boca_3);//mouth 3
    if (vol_leds>45)  copia_matriz(boca_4);//mouth 4
    //Serial.print(vol);
    //Serial.print(" ");
    //Serial.println(vol_leds);
    } else
      //Turn off the LEDs
      for (int j=0;j<NUM_LEDS;j++)
        leds[j] = CRGB::Black;
    FastLED.show();
    //delay(50);
}
