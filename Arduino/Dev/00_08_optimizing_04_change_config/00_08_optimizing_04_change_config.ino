////////////////////////////////////
// optimizing resources
// five masks
// Change configuration under the serial port
//
// "r123"   = Red 124       [0-255]
// "g123"   = Green 123     [0-255]
// "b123"   = Blue 123      [0-255]
// "m2"     = Select mask 2 [0-4]
// "i10"    = Intendisty 10 [0-255]
// "p"      = Play

//LIBRARIES

#include <FastLED.h>


//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8
#define DATA_PIN 8     //Pin DATA IN of the matrix
#define BRIGHTNESS 20  //Adjust the brightness of the led matrix
#define FIL 8
#define COL 8
#define COULEUR CRGB::Brown     //The selected color to paint
#define MASCARA_N 2             // Select the mask 0 to 4
#define BUF_LEN 20
#define ENTRE 50
#define ESPERA 500

//VARIABLES

//reducción de tamaño de mascaras a bits
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
//Copy n mouth with selected colour
void copia_boca(byte laboca[FIL], CRGB color){
  //snake-shaped matrix (zig-zag)
  int k=0;
  byte linea_boca;
  for(int x=0;x<8;x++){
    linea_boca = laboca[7-x];               //select the current row
    for(int y=0;y<8;y++){
      if (linea_boca & 0b00000001) {
        if ((x%2)==0) leds[k]= color;      //----> even row
        else leds[k+(7-y*2)]= color;       //<---- odd row
      }else leds[k]= 0;                    //Black
      k=k+1;
      linea_boca= linea_boca >> 1;         //shift a bit from the current row 
      }}
  }

//Talk
void sonrie(){
    //Opening mouth
    for(int j=0;j<5;j++){
      copia_boca(bocas[Conf.mascara][j],Conf.color);
      FastLED.show();
      delay(ENTRE);}

    for(int j=0;j<5;j++){
    //Closing mouth
      copia_boca(bocas[Conf.mascara][4-j],Conf.color);
      FastLED.show();
      delay(ENTRE);}
  }
 
//Off matrix
void apaga(){ 
      copia_boca(bocas[0][0],CRGB::Black);
      FastLED.show();
      delay(ESPERA);
  }

//Send configuration to serial port
void envia_config(){
    //
    Serial.println("Configuración actual:");
    //
    Serial.print("Color:");
    //Serial.println(Conf.color,HEX);
    Serial.print(Conf.color.r);
    Serial.print(" ");
    Serial.print(Conf.color.g);
    Serial.print(" ");
    Serial.println(Conf.color.b);
    //
    Serial.print("Intensidad:");
    Serial.println(Conf.intensidad);
    //
    Serial.print("Máscara:");
    Serial.println(Conf.mascara);
    //  
  }

//SETUPS
void setup() {
    //Initial configuration
    Conf.color      =    COULEUR;
    Conf.intensidad = BRIGHTNESS;
    Conf.mascara    = MASCARA_N;

    Serial.begin(9600);
    FastLED.addLeds<WS2812B, DATA_PIN,GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(  Conf.intensidad );
    
    envia_config();
    Serial.println("Esperando comandos");

}

//LOOP
void loop() {
    static char sdata[BUF_LEN], *pSdata=sdata;
    byte ch;
    int val;

     if (Serial.available()) {
        ch = Serial.read();
  
        // -1 for null terminator space
        if ((pSdata - sdata)>=BUF_LEN-1) {
           pSdata--; 
           Serial.print("BUFFER OVERRUN\n"); 
        }
  
        *pSdata++ = (char)ch;
  
        if (ch=='\r') {  // Command received and ready.
  
           pSdata--;       // Don't add \r to string.
           *pSdata = '\0';  // Null terminate the string.
  
           switch( sdata[0] ) {
           case 'm':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.mascara = val;}
              break;
           case 'r':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.color.r=val;}
              break;
           case 'g':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.color.g=val;}
              break;
           case 'b':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.color.b=val;}
              break;              
           case 'i':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.intensidad=val;}
                 FastLED.setBrightness(  Conf.intensidad );
              break;
           case 'p':
              Serial.println("Play máscara");
              sonrie();
              apaga();
              break;
              
           default: Serial.println(sdata);
           } // switch
           envia_config();
  
           pSdata = sdata;
  
        } // if \r

    }//else  //play mask ever
    //{
    //  sonrie();
    //  apaga();
    //  }
     
}
