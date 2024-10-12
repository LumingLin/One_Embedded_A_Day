// #include <Arduino.h>

// RTC_DATA_ATTR int bootCount = 0;

// //This line is unnecessary, since LED_BUILTIN is already defined in header files. 
// //I wrote it here for clearity.
// // const int LED_BUILTIN = 97; 

// void alarm(){
//   Serial.println("Human movement detected!!!");
//   digitalWrite(LED_BUILTIN, HIGH); //turn on LED

//   //This is not a good practice, since it will block the other processes from running
//   //We can use millis() in a loop instead
//   delay(5000);
//   digitalWrite(LED_BUILTIN, LOW); //turn off LED
// }

// void setup(){
//   Serial.begin(9600);
//   delay(1000); //Take some time to open up the Serial Monitor
//   pinMode(LED_BUILTIN, OUTPUT);

//   //Increment boot number and print it every reboot
//   ++bootCount;
//   Serial.println("Boot number: " + String(bootCount));

//   alarm();

//   esp_sleep_enable_ext0_wakeup(GPIO_NUM_1,1); //1 = High, 0 = Low

//   //Go to sleep now
//   Serial.println("Going to sleep now");
//   delay(1000); //Delay to prevent button bouncing and for Serial to finish transmitting
//   esp_deep_sleep_start();
// }

// void loop(){
//   //This is not going to be called
// }