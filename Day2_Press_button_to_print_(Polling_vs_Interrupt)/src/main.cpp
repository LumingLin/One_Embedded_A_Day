#include <Arduino.h>

const int GPIO1 = 1;
//We can also use the built-in boot button
//const int GPIO0 = 0;
bool buttonPressed = false;

// put function declarations here:
void ISR();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(GPIO1, INPUT);
  attachInterrupt(digitalPinToInterrupt(GPIO1), ISR, CHANGE);
}

void loop() {
  if(buttonPressed){
    Serial.println("Button is pressed.");
    buttonPressed = false;
  }
}

//Interrupt Service Routine
void ISR() {
  buttonPressed = true;
}

