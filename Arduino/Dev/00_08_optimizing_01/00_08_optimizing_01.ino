////////////////////////////////////
// optimizing resources
// We reduce 3 bytes by one bit
//

#include <FastLED.h>


//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS 20  //Adjust the brightness of the led matrix
#define FIL 8
#define COL 8
#define COULEUR CRGB::Blue     //The selected color to paint


//reducción de tamaño de mascaras a bits
//GLOBAL VARIABLES

//first mouth
//CRGB boca_0[FIL][COL] = {
//    {0,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,0,0},
//    {1,1,1,1,1,1,1,1},
//    {1,1,1,1,1,1,1,1},
//    {0,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,0,0}
//};
// One CRGB to on bit
// heght CRGB to one byte
// also
byte boca_0[FIL] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b11111111,
  0b11111111,
  0b00000000,
  0b00000000,
  0b00000000,
  };

//second mouth
byte boca_1[FIL] = {
  0b00000000,
  0b00000000,
  0b00011000,
  0b11100111,
  0b11100111,
  0b00011000,
  0b00000000,
  0b00000000,
  };

//third mouth
byte boca_2[FIL] = {
  0b00000000,
  0b00000000,
  0b01111110,
  0b10000001,
  0b10000001,
  0b01111110,
  0b00000000,
  0b00000000,
  };

//fourth mouth
byte boca_3[FIL] = {
  0b00000000,
  0b00111100,
  0b01000010,
  0b10000001,
  0b10000001,
  0b01000010,
  0b00111100,
  0b00000000
  };

//fith mouth
byte boca_4[FIL] = {
  0b00111100,
  0b01000010,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b01000010,
  0b00111100,
  };

//working LEDs
CRGB leds[NUM_LEDS];
float vol = 0;
const uint16_t samples = 128;

//FUNCTIONS
void copia_boca(byte laboca[FIL]){
  //snake-shaped matrix (zig-zag)
  int k=0;
  byte linea_boca;
  for(int x=0;x<8;x++){
    linea_boca = laboca[7-x];               //select the current row
    for(int y=0;y<8;y++){
      if (linea_boca & 0b00000001) {
        if ((x%2)==0) leds[k]= COULEUR;     //----> even row
        else leds[k+(7-y*2)]= COULEUR;      //<---- odd row
      }else leds[k]= 0;                     //Black
      k=k+1;
      linea_boca= linea_boca >> 1;          //shift a bit from the current row 
      }}
  }
  
//SETUP
void setup() {
    Serial.begin(9600);                                       //  setup serial
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    // Config matrix comunication with driver WS2812B
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
    if (vol_leds<15)  copia_boca(boca_0);//mouth 0 
    if (vol_leds>=15) copia_boca(boca_1);//mouth 1
    if (vol_leds>25)  copia_boca(boca_2);//mouth 2
    if (vol_leds>35)  copia_boca(boca_3);//mouth 3
    if (vol_leds>45)  copia_boca(boca_4);//mouth 4
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
