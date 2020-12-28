/*
  Led2rom.cpp - Library for flashing Morse code.
  Created by sakydogalo@gmail.com, december 2020.
  Released into the public domain.
*/
 
#include "Led2rom.h"
#include <EEPROM.h>
#include <FastLED.h>

Led2rom::Led2rom()
{
	init();
}//Led2rom

void Led2rom::init(){
	//TODO
	//LOAD CURRENT MASK
	//
  change_mask(0);
	Serial.begin(9600);									//Setup serial port	
  FastLED.addLeds<WS2812B, NPIN,GRB>(_leds, NLEDS);	//Setup driver matrix leds
  
  //TODO: DEPEND OF CURRENT MASK
  //FastLED.setBrightness(_config.brightness);			//Adjust brightness
  //FastLED.setBrightness(20);c
	}

//Write current brightness matrix in position i [0-9] 
void Led2rom::write_brightness(int i, int brightness)
{
  _dir = (i*45)+40;
  EEPROM.put( _dir,brightness);
}//write_brightness

//Write current color matrix in position i [0-9]
void Led2rom::write_colour(int i, CRGB colour)
{
  _dir = (i*45)+42;
  EEPROM.put( _dir,colour);
}//write_colour

//Reset mask[i] with data factory in eeprom
//i[0-4]
void Led2rom::reset_mask(int i)
{
	_dir = i*45;
	EEPROM.put( _dir, _mouths[i] );
}//reset_mask

//Write current mask in eeprom
//i[0-9]
void Led2rom::write_mask(int i)
{
	_dir = i*45;
	EEPROM.put( _dir, _mask );
}//write_mask

//Read mask from eeprom to current mask
//i[0-9]
void Led2rom::read_mask(int i)
{
	_dir = i*45;
	EEPROM.get( _dir, _mask );
}//read_mask

//Read brightness of mask from eeprom
void Led2rom::read_brightness(int i)
{
	_dir = (i*45)+40;
	EEPROM.get(_dir,_config.brightness);
}//read_i

//Read colour of mask from eeprom
void Led2rom::read_colour(int i)
{
	_dir = (i*45)+42;
	EEPROM.get(_dir,_config.colour);
}//read_c

//Change data mask
void Led2rom::change_mask(int i)
{
  read_mask(i);
  read_brightness(i);
  read_colour(i);
}

//Copy mouth with color to the led matrix
void Led2rom::copy_mouth(byte mouth[ROW], CRGB color){
	//snake-shaped matrix (zig-zag)
	int k=0;
	byte row_mouth;
  //coment all code -->/*
	for(int x=0;x<8;x++){
		row_mouth = mouth[7-x];					//select the current row
		for(int y=0;y<8;y++){
      //Serial.print(x);Serial.print(",");Serial.print(y);Serial.print("(");
			if (row_mouth & 0b00000001) {
				if ((x%2)==0){
					_leds[k]= color;			//----> even row
          //Serial.print(k);
          }
				else{
					_leds[k+(7-y*2)]= color;	//<---- odd row
          //Serial.print(k+(7-y*2));
				}
       //Serial.print("-");Serial.print(color);Serial.print(")");
			} else{
        if ((x%2)==0){
          _leds[k]= CRGB::Black;      //----> even row
          //Serial.print(k);
          }
        else{
          _leds[k+(7-y*2)]= CRGB::Black;  //<---- odd row
          //Serial.print(k+(7-y*2));
        }
        //Serial.print("-");Serial.print(CRGB::Black);Serial.print(")");
			}
				//_leds[k]= 0;					//Black
      //Serial.print(" "); 
			k +=1;
			row_mouth >>= 1;					//shift a bit from the current row
		}//for
    //Serial.println();
	}//for*/
}//copy_mouth

//Talk
void Led2rom::talk(){
    //Opening mouth
    for(int j=0;j<5;j++){
      copy_mouth(_mask[j],_config.colour);
      FastLED.show();
      delay(BETWEEN);
    }//for

    for(int j=0;j<5;j++){
    //Closing mouth

      copy_mouth(_mask[4-j],_config.colour);
      FastLED.show();
      delay(BETWEEN);
    }//for
}//talk

//One smile
void Led2rom::smile(int i){
  off();
  copy_mouth(_mask[i],_config.colour);
  FastLED.show();
  delay(BETWEEN);
  }

//smile depending on the value [0-100]
void Led2rom::display_smile(int vol_leds){
  //STATIC
  /*
  if (vol_leds<20)  smile(0);
  else
    if (vol_leds>80)  smile(4);
    else 
      if (vol_leds>60)  smile(3);
      else
        if (vol_leds>40)  smile(2);
        else
          if (vol_leds>=20) smile(1);
  */
  //DINAMIC/*
  if (vol_leds<20)  smile(0);
  if (vol_leds>=20) smile(1);
  if (vol_leds>40)  smile(2);
  if (vol_leds>60)  smile(3);
  if (vol_leds>80)  smile(4);//*/
  }

void Led2rom::set_brightness(int brightness){
	FastLED.setBrightness(brightness);
	}

//Off smiles
void Led2rom::off(){ 
      copy_mouth(_mask[0],CRGB::Black);
      FastLED.show();
  }


//Listen command
bool Led2rom::listen_command(){
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
                 _config.mask = val;
                 change_mask(val);}
              break;
           case 'r':
              //Change color red of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 _config.colour.r=val;}
              break;
           case 'g':
              //Change color green of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 _config.colour.g=val;}
              break;
           case 'b':
              //Change color green of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 _config.colour.b=val;}
              break;              
           case 'i':
              //change intensity of LEDs of the current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 _config.brightness=val;
                 set_brightness(_config.brightness);}
              break;
           case 'p':
              //test current mask
              Serial.println("Play máscara");
              talk();
              //l2r.off();
              break;
           case 'd':
              //test current mask
              if (strlen(sdata)>1){
                 Serial.println("Ver sonrisa");
                 val = atoi(&sdata[1]);
                 smile(val);}
              break;
              
           case 'c':
              send_config();
              break;
           case 's':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 write_mask(val);
                 write_brightness(val, _config.brightness);
                 write_colour(val, _config.colour);}
              break;
                 
           case 'w':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 write_mask(val);}
              break;
           case 'x':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 write_brightness(val, _config.brightness);}
              break;
           case 'y':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 write_colour(val, _config.colour);}
              break;
              
           case 'k':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 reset_mask(val);}
              break;
           } // switch
           pSdata = sdata;
        } // if \r
        return true;
     }
     return false;
}

//Send config
void Led2rom::send_config(){
	Serial.println("Configuración actual:");
    //RGB
    Serial.println("Color RGB:");
    Serial.println( "(" + String(_config.colour.r) + ", " + String(_config.colour.g) + ", " + String(_config.colour.b) + ")");
    //Brigthness
    Serial.print("Intensidad:");
    Serial.println(_config.brightness);
    //Mask
    Serial.print("Máscara:");
    Serial.println(_config.mask);
    //Data of mask
    for (int i=0;i<5;i++){
		Serial.print("{");
		for (int j=0;j<8;j++){
			if (j==0) Serial.print("0x");
			else Serial.print(",0x");
			Serial.print(_mask[i][j],HEX);
			if(j==7){
			  if(i==4) Serial.print("}");
			  else Serial.print("},");}
		}
		Serial.println();
	}
}

//Capture sample microphone
float Led2rom::microphone(int pin){
    float nvol = 0;
    for (int i = 0; i<SAMPLES; i++){
        auto analog = analogRead(pin);
        auto micline = abs(analog-512);
        nvol = max(micline, nvol);
    }
    vol = (nvol + 1.0*vol)/2.0;
    return vol;
  }
