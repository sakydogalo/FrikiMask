////////////////////////////////////
// optimizing resources
// five masks
//
// Change current mask configuration under the serial port
//
// "r123"   = Red 124       [0-255]
// "g123"   = Green 123     [0-255]
// "b123"   = Blue 123      [0-255]
// "m2"     = Select mask 2 [0-4]   Read complete mask from EEprom
// "c"      = view info current mask
// "d1"     = Display smile [0-4]
// "i10"    = Intendisty 10 [0-255] 
// "p"      = Play
//
//////////////////////
// k[0/1/2/3/4]
// reset mask i to factory
// s Save complete mask
// w Save mask
// x Save color
// y Save intensity

// chance smile of mask
// todo
// l[0/1/2/3/4] <-- 8Bytes           
//////////////////////
//TODO:
//sin límite de escritura en eeprom
//Memorizar mascara en uso
//Memorizar tiempos
//BUG. PUNTO
//BORRAR TODA LA EEPROM
//CON EEPROM VACIA ESCRIBIR MÁSCARAS INICIALES
//Port to library

//LIBRARIES
#include <FastLED.h>
#include <EEPROM.h>


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


//Current mask
byte boca[5][8]={};

//Masks factory for reset and if need reset
byte bocas[5][5][8]={
    {//BOCA1 ORIGINAL 
    {0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00},
    {0x00,0x00,0x18,0xE7,0xE7,0x18,0x00,0x00},
    {0x00,0x00,0x7E,0x81,0x81,0x7E,0x00,0x00},
    {0x00,0x3C,0x42,0x81,0x81,0x42,0x3C,0x00},
    {0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3C},
    },//BOCA1
  {//BOCA2 DARTH WADER
    {0x00,0x18,0x3C,0x42,0x81,0x81,0x81,0x5A},
    {0x00,0x18,0x3C,0x42,0x81,0x81,0x99,0x5A},
    {0x00,0x18,0x3C,0x42,0x81,0x99,0xA5,0x5A},
    {0x00,0x18,0x3C,0x42,0x99,0xA5,0xC3,0x5A},
    {0x00,0x18,0x3C,0x5A,0xA5,0xDB,0xA5,0x5A}
    },//BOCA2
  {//BOCA3 BENDER
    {0x00,0x00,0x7E,0x81,0xBC,0x81,0x7E,0x0},
    {0x00,0x00,0x7E,0x81,0xFF,0x91,0x7E,0x0},
    {0x00,0x00,0x7E,0x81,0xFF,0x99,0x7E,0x0},
    {0x00,0x00,0x7E,0x99,0xFF,0x99,0x7E,0x0},
    {0x00,0x00,0x7E,0xA5,0xFF,0xA5,0x7E,0x0} 
    },//BOCA3
  {//BOCA4 PACMAN 1
    {0x3C,0x42,0xA5,0x81,0x81,0x99,0x42,0x3C},
    {0x3C,0x42,0xA5,0x81,0x81,0xBD,0x42,0x3C},
    {0x3C,0x42,0xA5,0x81,0x99,0xBD,0x42,0x3C},
    {0x3C,0x42,0xA5,0x81,0x99,0xBD,0x5A,0x3C},
    {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x5A,0x3C}
    },//BOCA4
  {//BOCA5 PACMAN 2
    {0x3C,0x42,0xA5,0x81,0x81,0x81,0x5A,0x3C},
    {0x3C,0x42,0xA5,0x81,0x81,0x81,0x7E,0x3C},
    {0x3C,0x42,0xA5,0x81,0x81,0xBD,0x42,0x3C},
    {0x3C,0x42,0xA5,0x81,0xBD,0xC3,0x42,0x3C},
    {0x3C,0x42,0xA5,0xBD,0xC3,0x81,0x42,0x3C}
    }//BOCA5
    };

struct configuracion {
  CRGB color;
  int intensidad;
  int mascara;
  };

//current configuration
configuracion Conf;
//working LEDs
CRGB leds[NUM_LEDS];
float vol = 0;
const uint16_t samples = 128;

//FUNCTIONS
//Copy mouth with color to the led matrix
void copia_boca(byte laboca[FIL], CRGB color){
  int k=0;
  byte linea_boca;
  for(int x=0;x<8;x++){
    linea_boca = laboca[7-x];    
    for(int y=0;y<8;y++){
      if (linea_boca & 0b00000001) {
        if ((x%2)==0) leds[k]= color;
        else leds[k+(7-y*2)]= color;
      }else leds[k]= 0;
      k=k+1;
      linea_boca= linea_boca >> 1;
      }}
  }

//One talk
void sonrie(){   
    for(int j=0;j<5;j++){
      copia_boca(boca[j],Conf.color);
      FastLED.show();
      delay(ENTRE);}

    for(int j=0;j<5;j++){
      copia_boca(boca[4-j],Conf.color);
      FastLED.show();
      delay(ENTRE);}
  }

//One smile
void sonrisa(int i){
  apaga();
  copia_boca(boca[i],Conf.color);
  FastLED.show();
  delay(ESPERA);
  }
  
void apaga(){ 
      copia_boca(boca[0],CRGB::Black);
      FastLED.show();
  }

void envia_config(){

    Serial.println("Configuración actual:");
    //RGB
    Serial.println("Color RGB:");
    Serial.println( "(" + String(Conf.color.r) + ", " + String(Conf.color.g) + ", " + String(Conf.color.b) + ")");
    //Intensity
    Serial.print("Intensidad:");
    Serial.println(Conf.intensidad);
    //Mask
    Serial.print("Máscara:");
    Serial.println(Conf.mascara);
    //Data of mask
    for (int i=0;i<5;i++){
      Serial.print("{");
      for (int j=0;j<8;j++){
        if (j==0) Serial.print("0x");
        else Serial.print(",0x");
        Serial.print(boca[i][j],HEX);
        if(j==7){
          if(i==4) Serial.print("}");
          else Serial.print("},");}
      }
      Serial.println();
      }
  }

//Write current face[5 mouths]
//in eeprom in position i [0-9]
void graba_boca(int i){
  int direccion = i * ( 40 + 5);
  EEPROM.put( direccion, boca );
  }

//Reset face[5 mouths] in eeprom in position i [0-4]
//with default data
void reset_boca(int i){
  int direccion = i * ( 40 + 5);
  EEPROM.put( direccion, bocas[i] );
  }

//Write current intensity matrix in position i [0-9]
void graba_intensidad(int i, int intensidad){
  int direccion = i * ( 40 + 5);
  direccion += 40;
  EEPROM.put( direccion, intensidad);
  }

//Write current color matrix in position i [0-9]
void graba_color(int i, CRGB color){
  int direccion = i * ( 40 + 5);
  direccion += 42;
  EEPROM.put( direccion,color);
  }
  
//Read face[i]from eeprom [0-9]
//to the current mask
void lee_boca(int i){
  int direccion = i * ( 40 + 5);
  EEPROM.get( direccion,boca);
  }

//Read intensity[i]from eeprom [0-9]
//to the current mask
void lee_intensidad(int i){
  int direccion = i * ( 40 + 5);
  direccion += 40;
  EEPROM.get(direccion,Conf.intensidad);
  }

//Read color[i]from eeprom [0-9]
//to the current mask
void lee_color(int i){
  int direccion = i * ( 40 + 5);
  direccion += 42;
  EEPROM.get( direccion,Conf.color);
  }

//SETUP
void setup() {
    //inicialización de configuración
    Conf.color      =    COULEUR;
    Conf.intensidad = BRIGHTNESS;
    Conf.mascara    =         MASCARA_N;
    //
    Serial.begin(9600);
    FastLED.addLeds<WS2812B, DATA_PIN,GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(  Conf.intensidad );
    envia_config();
}

//LOOP
void loop() {
    //MUESTREO DE AUDIO
    float nvol = 0;
    
    for (int i = 0; i<samples; i++){
        auto analog = analogRead(A7);
        auto micline = abs(analog-512);
        nvol = max(micline, nvol);
    }
    vol = (nvol + 1.0*vol)/2.0;
    int vol_leds = map(vol, 200, 512, 0, NUM_LEDS);

    
    static char sdata[BUF_LEN], *pSdata=sdata;
    byte ch;
    int val;
     //INTERPRETES DE COMANDOS
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
              //change the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.mascara = val;
                 lee_boca(val);
                 lee_intensidad(val);
                 lee_color(val);}   
              break;
           case 'r':
              //Change color red of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.color.r=val;}
              break;
           case 'g':
              //Change color green of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.color.g=val;}
              break;
           case 'b':
              //Change color green of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.color.b=val;}
              break;              
           case 'i':
              //change intensity of LEDs of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Conf.intensidad=val;}
                 FastLED.setBrightness(  Conf.intensidad );
              break;
           case 'p':
              //test current mask
              Serial.println("Play máscara");
              sonrie();
              apaga();
              break;
           case 'd':
              //test current mask
              if (strlen(sdata)>1){
                 Serial.println("Ver sonrisa");
                 val = atoi(&sdata[1]);
                 sonrisa(val);}
              break;
              
           case 'c':
              envia_config();
              break;
           case 's':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 graba_boca(val);
                 graba_intensidad(val,Conf.intensidad);
                 graba_color(val,Conf.color);}
              break;
                 
           case 'w':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 graba_boca(val);}
              break;
           case 'x':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 graba_intensidad(val,Conf.intensidad);}
              break;
           case 'y':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 graba_color(val,Conf.color);}
              break;
              
           case 'k':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 reset_boca(val);}
              break;


           } // switch

           pSdata = sdata;
  
        } // if \r
        
        
        
     }else  // available
    {
          if(vol>200){
            //Serial.println(vol_leds);
            //for (int i = 0; i <= vol_leds; i++) {
             //       leds[i] = CRGB::Green;}
            if (vol_leds<15) copia_boca(boca[0],Conf.color);
            if (vol_leds>=15)copia_boca(boca[1],Conf.color);
            if (vol_leds>25) copia_boca(boca[2],Conf.color);
            if (vol_leds>35) copia_boca(boca[3],Conf.color);
            if (vol_leds>45) copia_boca(boca[4],Conf.color);
            FastLED.show();
            delay(ENTRE);
            } else {
                    apaga();
                        }
      }
     
}
