

// TODO
// chance smile of mask
// l[0/1/2/3/4] <-- 8Bytes           
// 
// Memorize current mask
// Add setup times
// Erase all eeprom
// Detect initial state, eeprom with out data and fill with mask CON EEPROM VACIA ESCRIBIR MÁSCARAS INICIALES
// SMILES DINAMIC OR STATIC
// SCREEN SAVER
// Other modes

//LIBRARIES
#include "Led2rom.h"

//CONSTANTS
#define CORTE 300
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

    float vol = l2r.microphone(A7);
    int vol_leds = map(vol, CORTE, 512, 0, TOPE);

    if(!l2r.listen_command()){
      if(vol>CORTE){
        Serial.println(String(vol)+"\t"+String(vol_leds));
        l2r.display_smile(vol_leds);
        } else {
          l2r.off();
          }
    }
}
