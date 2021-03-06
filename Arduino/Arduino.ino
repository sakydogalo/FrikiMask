/*
  Arduino.ino - 
  Created by sakydogalo@gmail.com, december 2020.
  Released into the public domain.
*/

// TODO
// chance smile of mask or complet mask (40 bytes)
// l[0/1/2/3/4] <-- 8Bytes           
// After save to eeprom
// Button functions
// Double press
// Choose color rgb
// Memorize current mask
// Erase all eeprom
// Detect initial state, eeprom with out data and fill with mask CON EEPROM VACIA ESCRIBIR MÁSCARAS INICIALES
// SMILES DINAMIC OR STATIC
// SCREEN SAVER
// Other modes

//LIBRARIES
#include <OneButton.h>
#include "Led2rom.h"

//CONSTANTS
#define CORTE 300
#define TOPE 100
#define BUTTONPIN  2

//OBJECTS AND VARIABLES
OneButton button(BUTTONPIN, true);
Led2rom l2r;

//FUNCTIONS
void shortPress(){
  //Serial.println("Short press");
  l2r.short_press();
  }
void doublePress(){
  //Serial.println("Double press");
  l2r.double_press();
  }
void longPress(){
  //Serial.println("Long press");
  l2r.long_press();
  }

//SETUP
void setup() {
	button.attachClick(shortPress);
	button.attachDoubleClick(doublePress);
	button.attachLongPressStart(longPress);
  l2r.init();
  l2r.send_config();
}

//LOOP
void loop() {
	button.tick();
    float vol = l2r.microphone(A7);
    int vol_leds = map(vol, CORTE, 512, 0, TOPE);
	if(l2r.menu==0){ //Only in main menu listen comands via serial port
		if(!l2r.listen_command()){
		  if(vol>CORTE){
			Serial.println(String(vol)+"\t"+String(vol_leds));
			l2r.display_smile(vol_leds);
			} else {
			  l2r.off();
			  }
		}
    }//if l2r.menu==0
    //l2r.listen_button();
    
    if(l2r.menu==1) l2r.plus_brightness();
    if(l2r.menu==2) l2r.plus_colour();

}

/*test button simple
  //setup
  pinMode(2, INPUT_PULLUP); 
  //int buttonStatus = 0;
  //loop
  int pinValue = digitalRead(2);
  delay(10); // quick and dirty debounce filter
  if (buttonStatus != pinValue) {
    buttonStatus = pinValue;
    Serial.println(buttonStatus);
  }*/ 
