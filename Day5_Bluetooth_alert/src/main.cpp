/*
  ESP32 Bluetooth Low Energy (BLE) Server Example
  This code sets up an ESP32 as a BLE server with an Alert Notification Service.
  It allows clients to connect, read, write, and receive notifications.

  References:
  - Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  - Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLETests/SampleNotify.cpp
  - Ported to Arduino ESP32 by Evandro Copercini
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// BLE service and characteristic UUIDs
// Generate your own UUIDs at https://www.uuidgenerator.net/ if needed
#define SERVICE_UUID        "1811" // UUID for Alert Notification Service
#define CHARACTERISTIC_UUID "2A46" // UUID for New Alert

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
BLEServer *pServer; // Make pServer a global variable

// Callback class to handle server events
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device Connected!");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device Disconnected!");
    pServer->startAdvertising(); // Restart advertising immediately after disconnection
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE server...");

  //Create the BLE Device
  BLEDevice::init("ESP32_Wroom_BLE_Server");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  
  //Create a BLE Descriptor, this can be used by the client to notify the server about whether to notify the client or not
  //Which is useful to save power for both the client and the server
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Configuring the advertising
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  // BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  // pAdvertising->addServiceUUID(SERVICE_UUID);
  // pAdvertising->setScanResponse(true);
  // pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  // pAdvertising->setMinPreferred(0x12);

  //Start advertising
  //BLEDevice::startAdvertising();

  //Another way to start advertising
  pServer->getAdvertising()->start();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // Set an initial value for the characteristic
  pCharacteristic->setValue("ESP32 BLE Server");
}

void loop() {
  static unsigned long lastNotifyTime = 0;
  static int counter = 0;

  if (deviceConnected) {
    unsigned long currentTime = millis();
    
    // Send a notification every 1 second
    if (currentTime - lastNotifyTime >= 1000) {
      String message = "Count: " + String(counter);
      pCharacteristic->setValue(message.c_str());
      pCharacteristic->notify();
      Serial.println("Notified: " + message);
      
      counter++;
      lastNotifyTime = currentTime;
    }
  } else {
    // Reset counter when disconnected
    counter = 0;
    delay(1000);
  }
}
