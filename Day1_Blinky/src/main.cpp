#include <Arduino.h>

/*
  Simple Blink sketch
  simple-blink.cpp
  Use for PlatformIO demo

  From original Arduino Blink Sketch
  https://www.arduino.cc/en/Tutorial/Blink
  
  DroneBot Workshop 2021
  https://dronebotworkshop.com
*/

// Set LED_BUILTIN if undefined or not pin 97
// #define LED_BUILTIN 97

void setup()
{
  // Initialize LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  // Set the LED HIGH
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("HIGH");

  // Wait for a second
  delay(1000);

  // Set the LED LOW
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LOW");

   // Wait for a second
  delay(1000);
}