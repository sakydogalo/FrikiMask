/*
  Led2rom.h - Library Library Led2rom.
  Created by sakydogalo@gmail.com, december 2020.
  Released into the public domain.
*/
#ifndef Led2rom_h
#define Led2rom_h

#define NLEDS     64 
#define DATAPIN    8
#define BETWEEN   50
#define SAMPLES  128
#define BUF_LEN   20
#define MAXFLASH   5
#define MAXEEPROM  5
#define MAXMENU    4

#include <EEPROM.h>
#include <FastLED.h>
#include "millisDelay.h"
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
    //menu and submenu
    uint8_t menu = 0;
    uint8_t submenu= 0;
    //
    boolean add_brightness = false;
    boolean stop_brightness = false;
    uint8_t brightness_tmp = 0;
    millisDelay brightnessDelay;
    //
    boolean add_colour = false;
    boolean stop_colour = false;
    uint8_t rgb_r_tmp = 0;
    uint8_t rgb_g_tmp = 0;
    uint8_t rgb_b_tmp = 0;
    CRGB colour_tmp = CRGB::Black;
    char rgb_tmp = 'r';
    millisDelay colourDelay;
    //
    Led2rom();
    void init();
    //WRITE TO EEPROM
    void r_mask_flash(uint8_t i);
    void r_mask_ee(uint8_t i);
    void w_mask_ee(uint8_t i);
    //
    void rst_mask_ee(uint8_t i);
    void factory_ee();
    
    void copy_mouth(byte mouth[8], CRGB color);//TODO: change by reference
    void draw_colour(byte mouth[8], CRGB color);//TODO: change by reference
    
    //for matrix leds
    void talk();
    void smile(int i);
    void display_smile(int vol_leds);
    void black();
    void off();
    void set_brightness(uint8_t brightness);
    //for serial
    void send_config();
    bool listen_command();
    //for button
    // void listen_button()
    void long_press();
    void short_press();
    void double_press();
    void togle_mask_ee();
    void togle_mask_flash();
    void plus_brightness();
    void plus_colour();
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
    //
    boolean buttonActive = false;
    boolean longPressActive = false;
    long buttonTimer = 0;
    long longPressTime = 1000;

    //
    uint8_t current_i_flash = 0;    //Current index mask from flash
    uint8_t current_i_ee= 0;        //Current index mask from eeprom
    byte display_rgb [4][8]{
			{ 0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 ,0b00000011 ,0b00000011 ,0b00000011 ,0b00000011 },
			{ 0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 ,0b00011000 ,0b00011000 ,0b00011000 ,0b00011000 },
			{ 0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 ,0b11000000 ,0b11000000 ,0b11000000 ,0b11000000 },
			{ 0b11111111 ,0b11111111 ,0b11111111 ,0b11111111 ,0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 }		
		};

  /*
 * 
* R { 0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 ,0b00000011 ,0b00000011 ,0b00000011 ,0b00000011 }
* G { 0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 ,0b00011000 ,0b00011000 ,0b00011000 ,0b00011000 }
* B { 0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 ,0b11000000 ,0b11000000 ,0b11000000 ,0b11000000 }
* RGB { 0b11111111 ,0b11111111 ,0b11111111 ,0b11111111 ,0b00000000 ,0b00000000 ,0b00000000 ,0b00000000 }
* */
};//Led2rom
#endif
