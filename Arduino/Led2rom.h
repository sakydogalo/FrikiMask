/*
  Led2rom.h - Library for flashing Morse code.
  Created by sakydogalo@gmail.com, december 2020.
  Released into the public domain.
*/
#ifndef Led2rom_h
#define Led2rom_h


#define ROW       8
#define NLEDS    64 
#define NPIN      8
#define BETWEEN  50
#define SAMPLES 128
#define BUF_LEN  20

#include <EEPROM.h>
#include <FastLED.h>
#include "Masks.h"

struct configuration {
  uint8_t brightness;
  CRGB colour;
  uint8_t mask;
  };

class Led2rom
{
  public:
    configuration _config; //Current configuration
    Led2rom();
    void init();
    //WRITE TO EEPROM
    void r_mask_flash(uint8_t i);
    void r_mask_ee(uint8_t i);
    void w_mask_ee(uint8_t i);
    //
    void rst_mask_ee(uint8_t i);
    void factory_ee();
    
    void copy_mouth(byte mouth[ROW], CRGB color);
    //for matrix leds
    void talk();
    void smile(int i);
    void display_smile(int vol_leds);
    void black();
    void off();
    void set_brightness(int brightness);
    //for serial
    void send_config();
    bool listen_command();
    //for microphone
    float microphone(int pin);
    
  private:
    int _dir;//¿need?
    float vol = 0;
    bool dinamic = false;
  	CRGB _leds[NLEDS] = {    //working leds
  		};
  
      byte _mask[5][8] = {     //Current mask
  		};
      
};//Led2rom
#endif
