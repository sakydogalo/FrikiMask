////////////////////////////////////
//
// Testing microphone on input A7
// Sending raw and processed read through serial port

//LIBRARIES

//CONSTANTS
#define NUM_LEDS 64    //Number of LEDs in the matrix 8X8

//SETUP
void setup() {
  Serial.begin(9600);           //  setup serial
}

//VARIABLES
float vol = 0;
const uint16_t samples = 128;

//LOOP
void loop() {
  float nvol = 0;
  
  //128 sample capture
  for (int i = 0; i<samples; i++){
      auto analog = analogRead(A7);
      auto micline = abs(analog-512);
      nvol = max(micline, nvol);
  }
  
  vol = (nvol + 1.0*vol)/2.0;
  //mapping of values ​​for led matrix
  int vol_leds = map(vol, 200, 512, 0, NUM_LEDS);
  if(vol>200){
    //sending through the serial port
    Serial.print(vol);
    Serial.print(" ");
    Serial.print(nvol);
    Serial.print(" ");
    Serial.println(vol_leds);}

}
