#include <OneButton.h>

OneButton button(2, true);

// setup code here, to run once.
void setup() {
  button.attachClick(myClickFunction);
  button.attachDoubleClick(myDoubleClickFunction);
  button.attachLongPressStart(myLongPressStartFunction);
  button.setDebounceTicks(80);

  Serial.begin(9600); 
} // setup


// main code here, to run repeatedly: 
void loop() {

  button.tick();

} // loop


// this function will be called when the button was pressed 1 time and them some time has passed.
void myClickFunction() {
  Serial.println("Click");
} // myClickFunction


// this function will be called when the button was pressed 2 times in a short timeframe.
void myDoubleClickFunction() {
  Serial.println("Double click");
} // myDoubleClickFunction

void myLongPressStartFunction(){
  Serial.println("Long click");
  }
// End
