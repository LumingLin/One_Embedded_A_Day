#include <Arduino.h>
#include <BLEServer.h>
#include <BLEDevice.h>
// #include <BLEUtils.h>
#include <BLE2902.h>

//PIR Sensor & Interrupts
volatile bool humanMovement = false; // Flag to indicate human movement detected
volatile bool timerRunning = false;   // Flag to indicate if the timer is running
const uint8_t PIR_SENSOR = 2;         // Pin number for the PIR sensor
const unsigned long TIME_SECONDS = 5;  // Duration to keep the LED on (in seconds)
unsigned long startTime = 0;          // Variable to store the start time of the timer
unsigned long lastPrintTime = 0;      // Variable to store the last print time

void IRAM_ATTR alarm() {
  humanMovement = true; // Set flag when motion is detected
}

//Bluetooth Settings
#define SERVICE_UUID        "1811" // UUID for Alert Notification Service
#define CHARACTERISTIC_UUID "2A46" // UUID for New Alert

BLECharacteristic *pCharacteristic; // Pointer to the BLE characteristic
bool deviceConnected = false;        // Flag to indicate if a device is connected
BLEServer *pServer;                  // Pointer to the BLE server

class MyServerCallbacks: public BLEServerCallbacks{
  void onConnect(BLEServer* pServer){
    deviceConnected = true; // Set flag when a device connects
    Serial.println("Device Connected!"); // Log connection
  }

  void onDisconnect(BLEServer* pServer){
    deviceConnected = false; // Set flag when a device disconnects
    Serial.println("Device Disconnected!"); // Log disconnection
    pServer->startAdvertising(); // Restart advertising for new connections
  }
};

void BLE_notify(String message){
    try{
      if(deviceConnected) {
        pCharacteristic->setValue(message.c_str()); 
        pCharacteristic->notify(); 
        Serial.println("BLE notification sent."); 
      } else {
        throw 404; // Throw error if not connected
      }
    }
    catch(int ErrorCode){
      switch (ErrorCode)
      {
      case 404:
        Serial.println("Failed to notify: Bluetooth Not Connected.");
        break;
      
      default:
        Serial.println("Something happened..."); // Log unknown error
        break;
      }
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_SENSOR, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(PIR_SENSOR), alarm, HIGH);

  //Setting up Bluetooth
  Serial.println("Starting BLE Server...");

  //Create the BLE Device
  BLEDevice::init("ESP32_Wroom_BLE_Server");

  //Create BLE Server
  pServer = BLEDevice::createServer(); 
  pServer->setCallbacks(new MyServerCallbacks()); // Set callbacks for server events

  //Create the BLE Service
  BLEService *pService = pServer -> createService(SERVICE_UUID);

  //Create a BLE Characterisitc
  pCharacteristic = pService -> createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic -> addDescriptor(new BLE2902());

  //Start the service
  pService -> start();

  //Start advertising
  pServer->getAdvertising()->start(); // Start advertising for connections
  pCharacteristic->setValue("ESP32 BLE Server"); // Set initial value for characteristic

  Serial.println("Setup complete");
}

void loop() {
  unsigned long currentMillis = millis();

  String message = "Empty Message"; // Default message
  if (humanMovement && !timerRunning) { // Check if movement is detected and timer is not running
    message = "Human Movement Detected!"; // Update message
    
    Serial.println(message);
    digitalWrite(LED_BUILTIN, HIGH);

    //Send Bluetooth Notification
    BLE_notify(message); // Notify connected devices of movement

    startTime = currentMillis; // Record the start time
    timerRunning = true; // Set timer running flag
    humanMovement = false; // Reset movement flag immediately
  }

  if (timerRunning && (currentMillis - startTime >= TIME_SECONDS * 1000)) { // Check if timer has expired
    message = "Turning LED off..."; 
    Serial.println(message); 
    digitalWrite(LED_BUILTIN, LOW); 
   
    //Send Bluetooth Notification
    BLE_notify(message); 
    timerRunning = false; 
  }
}
