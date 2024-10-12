#include <Arduino.h>

volatile bool humanMovement = false;
volatile bool timerRunning = false;
const uint8_t PIR_SENSOR = 1;
const unsigned long TIME_SECONDS = 5; // turn on the LED for 5 sec
unsigned long startTime = 0;
unsigned long lastPrintTime = 0;

void IRAM_ATTR alarm() {
  humanMovement = true;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_SENSOR, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(PIR_SENSOR), alarm, HIGH);
  
  Serial.println("Setup complete");
}

void loop() {
  unsigned long currentMillis = millis();

  if (humanMovement && !timerRunning) {
    Serial.println("Human Movement Detected!");
    digitalWrite(LED_BUILTIN, HIGH);
    startTime = currentMillis;
    timerRunning = true;
    humanMovement = false; // Reset flag immediately
  }

  if (timerRunning && (currentMillis - startTime >= TIME_SECONDS * 1000)) {
    Serial.println("Turning LED off...");
    digitalWrite(LED_BUILTIN, LOW);
    timerRunning = false;
  }

  if (currentMillis - lastPrintTime >= 100) {  // Print every 1 second
    Serial.print("PIR Sensor State: ");
    Serial.println(digitalRead(PIR_SENSOR));
    lastPrintTime = currentMillis;
  }
}
