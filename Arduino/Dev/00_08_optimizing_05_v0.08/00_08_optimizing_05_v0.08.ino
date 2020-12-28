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

#include "Led2rom.h"

//CONSTANTS
#define BUF_LEN 20
#define ENTRE 50
#define TOPE 100
//OBJECTS AND VARIABLES




Led2rom l2r;
//FUNCTIONS
//SETUP
void setup() {
    l2r.init();
    l2r.send_config();
}

//LOOP
void loop() {
    //MUESTREO DE AUDIO
    
    //Serial.println(l2r.microphone(A7));
    ;
    l2r.microphone(A7);
    int vol_leds = map(l2r.vol, 300, 512, 0, TOPE);
    //Serial.println(String(l2r.vol)+"\t"+String(vol_leds));

    
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
           Serial.println(sdata);
           switch( sdata[0] ) {
           case 'm':
              //change the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r._config.mask = val;
                 l2r.change_mask(val);}
              break;
           case 'r':
              //Change color red of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r._config.colour.r=val;}
              break;
           case 'g':
              //Change color green of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r._config.colour.g=val;}
              break;
           case 'b':
              //Change color green of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r._config.colour.b=val;}
              break;              
           case 'i':
              //change intensity of LEDs of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r._config.brightness=val;
                 l2r.set_brightness(l2r._config.brightness);}
              break;
           case 'p':
              //test current mask
              Serial.println("Play máscara");
              l2r.talk();
              //l2r.off();
              break;
           case 'd':
              //test current mask
              if (strlen(sdata)>1){
                 Serial.println("Ver sonrisa");
                 val = atoi(&sdata[1]);
                 l2r.smile(val);}
              break;
              
           case 'c':
              l2r.send_config();
              break;
           case 's':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r.write_mask(val);
                 l2r.write_brightness(val, l2r._config.brightness);
                 l2r.write_colour(val, l2r._config.colour);}
              break;
                 
           case 'w':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r.write_mask(val);}
              break;
           case 'x':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r.write_brightness(val, l2r._config.brightness);}
              break;
           case 'y':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r.write_colour(val, l2r._config.colour);}
              break;
              
           case 'k':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 l2r.reset_mask(val);}
              break;
           

           } // switch

           pSdata = sdata;
  
        } // if \r

     } else  // available
    {
      if(l2r.vol>300){
        //Serial.println(String(l2r.vol)+"\t"+String(vol_leds));
        l2r.display_smile(vol_leds);
        } else {
          l2r.off();
          }
      }
     
}
