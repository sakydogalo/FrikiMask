////////////////////////////////////
// optimizing resources
// five masks
//

#include <FastLED.h>

//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS 20  //Adjust the brightness of the led matrix
#define FIL 8
#define COL 8
#define COULEUR CRGB::Brown     //The selected color to paint
#define MASCARA_N 2             // Select the mask 0 to 4

#define ENTRE 50
#define ESPERA 500

//VARIABLES

//Five masks
byte bocas[5][5][8]={ //ORIGINAL, DARTH VADER, PACMAN IZQ, PACMAN DER, BENDER
  {//BOCA1 ORIGINAL 
    {0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00},
    {0x00,0x00,0x18,0xe7,0xe7,0x18,0x00,0x00},
    {0x00,0x00,0x7e,0x81,0x81,0x7e,0x00,0x00},
    {0x00,0x3c,0x42,0x81,0x81,0x42,0x3c,0x00},
    {0x3c,0x42,0x81,0x81,0x81,0x81,0x42,0x3c}
    },//BOCA1
  {//BOCA2 DARTH WADER
    { 0b00000000,0b00011000,0b00111100,0b01000010,0b10000001,0b10000001,0b10000001,0b01011010},
    { 0b00000000,0b00011000,0b00111100,0b01000010,0b10000001,0b10000001,0b10011001,0b01011010},
    { 0b00000000,0b00011000,0b00111100,0b01000010,0b10000001,0b10011001,0b10100101,0b01011010},
    { 0b00000000,0b00011000,0b00111100,0b01000010,0b10011001,0b10100101,0b11000011,0b01011010},
    { 0b00000000,0b00011000,0b00111100,0b01011010,0b10100101,0b11011011,0b10100101,0b01011010}
    },//BOCA2
  {//BOCA3 BENDER
    { 0b00000000,0b00000000,0b01111110,0b10000001,0b10110101,0b10000001,0b01111110,0b00000000},
    { 0b00000000,0b00000000,0b01111110,0b10000001,0b11111111,0b10010001,0b01111110,0b00000000},
    { 0b00000000,0b00000000,0b01111110,0b10000001,0b11111111,0b10011001,0b01111110,0b00000000},
    { 0b00000000,0b00000000,0b01111110,0b10011001,0b11111111,0b10011001,0b01111110,0b00000000},
    { 0b00000000,0b00000000,0b01111110,0b10100101,0b11111111,0b10100101,0b01111110,0b00000000}
    },//BOCA3
  {//BOCA4 PACMAN 1
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10000001,0b10011001,0b01000010,0b00111100},
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10000001,0b10111101,0b01000010,0b00111100},
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10011001,0b10111101,0b01000010,0b00111100},
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10011001,0b10111101,0b01011010,0b00111100},
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10011001,0b10100101,0b01011010,0b00111100},
    },//BOCA4
 
 {//BOCA5 PACMAN 2
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10000001,0b10000001,0b01011010,0b00111100},    
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10000001,0b10000001,0b01111110,0b00111100},
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10000001,0b10111101,0b01000010,0b00111100},
    { 0b00111100,0b01000010,0b10100101,0b10000001,0b10111101,0b11000011,0b01000010,0b00111100},      
    { 0b00111100,0b01000010,0b10100101,0b10111101,0b11000011,0b10000001,0b01000010,0b00111100},
    }//BOCA5
    };
    
//working LEDs
CRGB leds[NUM_LEDS];
float vol = 0;
const uint16_t samples = 128;

// struct Configuration mask
struct configuracion {
  CRGB color;
  int intensidad;
  int mascara;
  };

configuracion Conf;


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

void sonrie(){
    //Opening mouth
    for(int j=0;j<5;j++){
      copia_boca(bocas[Conf.mascara][j]);
      FastLED.show();
      delay(ENTRE);}
    //Closing mouth
    for(int j=0;j<5;j++){
      copia_boca(bocas[Conf.mascara][4-j]);
      FastLED.show();
      delay(ENTRE);}
  }

void apaga(){
      for (int j=0;j<NUM_LEDS;j++) leds[j] = CRGB::Black;
      FastLED.show();
      delay(ESPERA);
  }

//SETUP
void setup() {
    //inicialización de configuración
    Conf.color      =    COULEUR;
    Conf.intensidad = BRIGHTNESS;
    Conf.mascara    = MASCARA_N;

    Serial.begin(9600);
    FastLED.addLeds<WS2812B, DATA_PIN,GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(  Conf.intensidad );
    //
    Serial.println("Configuración actual:");
    //
    Serial.print("Color:");
    Serial.println(Conf.color);
    //
    Serial.print("Intensidad:");
    Serial.println(Conf.intensidad);
    //
    Serial.print("Máscara:");
    Serial.println(Conf.mascara);
    //
}

//LOOP
void loop() {
    sonrie();
    apaga();

}
