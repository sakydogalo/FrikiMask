/*
  Led2rom.cpp - Library for flashing Morse code.
  Created by sakydogalo@gmail.com, december 2020.
  Released into the public domain.
*/
 
#include "Led2rom.h"

Led2rom::Led2rom()
{
//	init();
}//Led2rom

void Led2rom::init(){
	//TODO
	//LOAD CURRENT MASK AND BRIGHTNESS

 
  Serial.begin(9600);                               //Setup serial port 
  r_mask_flash(4);                                  //TODO: load from eeprom de current mask(eeprom or flash)?
  FastLED.addLeds<WS2812B, DATAPIN,GRB>(_leds, NLEDS);	//Setup driver matrix leds
  //TODO: DEPEND OF CURRENT MASK
  FastLED.setBrightness(_config.brightness);        //TODO: set brightness from mask loaded previously

  pinMode(2, INPUT_PULLUP);                         //Setup the pinmode for the button
	}


//Read complete mask from flash
void Led2rom::r_mask_flash(uint8_t i){
    short direccion = &_mouths;
    uint8_t B;
    for(int x= 0; x<5; x++){
      for(int y= 0; y<8; y++) {
        B = pgm_read_byte( direccion+i*40+x*8+y);
        _mask[x][y]=B;
        }
      }

    direccion = &_colours;
    B = pgm_read_byte( direccion+i*3);
    _config.colour.r=B;
    
    B = pgm_read_byte( direccion+i*3+1);
    _config.colour.g=B;
    
    B = pgm_read_byte( direccion+i*3+2);
    _config.colour.b=B;
    
    direccion = &_brightness;
    B = pgm_read_byte( direccion+i);
    _config.brightness =2;
    
    _config.mask = i;
  }
  
//Read complete mask from eeprom
void Led2rom::r_mask_ee(uint8_t i){
	//Read mask from eeprom
	_dir = i*44;
	EEPROM.get( _dir, _mask );
	//Read brightness  from eeprom
	_dir += 40;
	EEPROM.get(_dir,_config.brightness);
	set_brightness(_config.brightness);
	//Read colour  from eeprom
	_dir += 1;
	EEPROM.get(_dir,_config.colour);
	
	_config.mask = i;
	}

//Write complete mask to eeprom
void Led2rom::w_mask_ee(uint8_t i){
	//Write current mask
	_dir = i*44;
	EEPROM.put( _dir, _mask );
	//write current brightness
	_dir += 40;
	EEPROM.put( _dir, _config.brightness);
	//Write current color
	_dir += 1;
	EEPROM.put( _dir, _config.colour);
}//write_mask

//Reset mask in eeprom
void Led2rom::rst_mask_ee(uint8_t i)
{
	//read from flash
	r_mask_flash(i);
	//write to eeprom
	w_mask_ee(i);
}//reset_mask

//Reset all eeprom with data factory "5 mask"
void Led2rom::factory_ee(){
	for(uint8_t i = 0;i<5;i++)
		rst_mask_ee(i);
	}//factory ee

//Copy mouth with color to the led matrix
void Led2rom::copy_mouth(byte mouth[8], CRGB color){
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
    for(int i = 0;i<5;i++){
    //Opening mouth
    for(int j=0;j<5;j++){
      smile(j);
      //copy_mouth(_mask[j],_config.colour);
      //FastLED.show();
      //delay(BETWEEN);
    }//for

    for(int j=0;j<5;j++){
    //Closing mouth
      smile(4-j);
      //copy_mouth(_mask[4-j],_config.colour);
      //FastLED.show();
      //delay(BETWEEN);
    }//for
    }//for i
}//talk

//One smile
void Led2rom::smile(int i){
  black();
  copy_mouth(_mask[i],_config.colour);
  FastLED.show();
  delay(BETWEEN);
  }

//smile depending on the value [0-100]
void Led2rom::display_smile(int vol_leds){
  if(dinamic){
	  if (vol_leds<20)  smile(0);
	  if (vol_leds>=20) smile(1);
	  if (vol_leds>40)  smile(2);
	  if (vol_leds>60)  smile(3);
	  if (vol_leds>80)  smile(4);}
  else {
	    if (vol_leds<20)  smile(0);
		  else
			if (vol_leds>80)  smile(4);
			else 
			  if (vol_leds>60)  smile(3);
			  else
				if (vol_leds>40)  smile(2);
				else
				  if (vol_leds>=20) smile(1);}
  }

void Led2rom::set_brightness(uint8_t brightness){
	//_config.brightness=brightness;
	FastLED.setBrightness(brightness);
	}

//Off smiles
void Led2rom::off(){ 
      black();
      FastLED.show();
  }

void Led2rom::black(){
      copy_mouth(_mask[0],CRGB::Black);
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
                 _config.brightness = val;
                 set_brightness(val);}
              break;
                            
           case 'p':
              //test current mask
              Serial.println("Play mask");
              talk();
              //l2r.off();
              break;
           case 'd':
              //test current mask
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 Serial.println("View mouth " + String(val));
                 smile(val);
                 delay(2000);}
              break;              
           case 'c':
              send_config();
              break;

           //Load and save masks
           case 'f':
              //change the current mask from flash 
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 r_mask_flash(val);}
              break;
            case 'e':
              //change the current mask from eeprom 
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 r_mask_ee(val);}
              break;
              
           case 's':
              //Save the current mask to eeprom
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 w_mask_ee(val);}
              break;

           case 'k':
              if (strlen(sdata)>1){
                 val = atoi(&sdata[1]);
                 rst_mask_ee(val);}
              break;

           case 'w':
              factory_ee();
              break;
           case 'D':
		      dinamic=true;
			  break;
           case 'S':
		      dinamic=false;
              break;   
           } // switch
           pSdata = sdata;
        } // if \r
        return true;
     }
     return false;
}//listen command

//listen button
void Led2rom::listen_button(){
    if (digitalRead(BUTTONPIN) == LOW) {
      if (buttonActive == false) {
        buttonActive = true;
        buttonTimer = millis();}
      if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
        longPressActive = true;
        long_press();}
    }//digitalRead(BUTTONPIN) == LOW
    else {
      if (buttonActive == true) {
        if (longPressActive == true) {
          longPressActive = false;}
        else {
          short_press();}
        buttonActive = false;}
    }
}//Listen button

//Menu and submenu
void Led2rom::long_press(){
  Serial.println("Long press");
  /*0 main menu
      * 1 choose britghtness
      * 2 choose colour
      * 3 select masc flash
      * 4
      * */
  menu +=1;
  if(menu>=MAXMENU+1) menu=0;
  Serial.println("Menu :"+String(menu));
  //optimizing?
  if(menu==0) Serial.println("Main");
  if(menu==1) Serial.println("Choose brightness mask");
  if(menu==2) Serial.println("Choose  colour mask");
  if(menu==3) Serial.println("Select mask from eeprom");
  if(menu==4) Serial.println("Select mask from flash");
  
}//long press

void Led2rom::plus_brightness(){
	if(!stop_brightness){
		if(!add_brightness ){
			brightness_tmp += 10;
			if(brightness_tmp>=250) add_brightness=true;
			}
		else {
			brightness_tmp -= 10;
			if(brightness_tmp<=0) add_brightness=false;
			}
		
		set_brightness(brightness_tmp);
		smile(0);
		Serial.println(brightness_tmp);
		//delay(100);
	}
}

void Led2rom::plus_colour(){
	
	if(!stop_colour){
		if(!add_colour ){
			//increase colour
			colour_tmp.r += 25;
			
			if(colour_tmp.r>=250){
				colour_tmp.g += 25;
				colour_tmp.r = 0;
				
				if(colour_tmp.g>=250){
					colour_tmp.b += 25;
					colour_tmp.g = 0;
					
					if(colour_tmp.b>=250){
						colour_tmp.b = 0;
						
						stop_colour = true;
						}
				}
			}
		}
	_config.colour = colour_tmp;
	smile(0);
	Serial.println( "(" + String(_config.colour.r) + ", " + String(_config.colour.g) + ", " + String(_config.colour.b) + ")");
	}
	
}
	
void Led2rom::short_press(){
  Serial.println("Short press");
  if(menu==3)   togle_mask_ee();
  if(menu==4)   togle_mask_flash();
  if(menu==1) stop_brightness = !stop_brightness;
  if(menu==2) stop_colour = !stop_colour;
  
}//short press

void Led2rom::double_press(){
  
}//double press

void Led2rom::togle_mask_ee(){
  current_i_ee +=1;
  if (current_i_ee>=MAXEEPROM) current_i_ee = 0;
  Serial.println("eeprom :"+ String(current_i_ee));
  r_mask_ee(current_i_ee);
  Serial.println("Play mask");
  talk();
}//togle mask from eeprom

void Led2rom::togle_mask_flash(){
  current_i_flash +=1;
  if (current_i_flash>=MAXFLASH) current_i_flash = 0;
  Serial.println("flash :"+ String(current_i_flash));
  r_mask_flash(current_i_flash);
  Serial.println("Play mask");
  talk();
  
}//togle mask from flash

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
