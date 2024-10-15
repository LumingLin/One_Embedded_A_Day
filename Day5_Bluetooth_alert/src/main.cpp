/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "1811" //UUID for Alert Notification Service
#define CHARACTERISTIC_UUID "2A46" //UUID for New Alert

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;



class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE work!");

  //Create the BLE Device
  BLEDevice::init("ESP32_Wroom");

  //Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  //Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                        ); //Read Write Notify and Indicate

  
  //Create a BLE Descriptor, this can be used by the client to notify the server about whether to notify the client or not
  //Which is useful to save power for both the client and the server
  pCharacteristic->addDescriptor(new BLE2902());

  //Start the service
  pService->start();

  
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  //Configure the advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);

  //Start advertising
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // Set an initial value for the characteristic
  pCharacteristic->setValue("No activity");
}

void loop() {
  static int humanActivity = 0;
  if (deviceConnected) {
    String alertMessage = "Alert: Human activity detected!!! " + String(humanActivity);
    pCharacteristic->setValue(alertMessage.c_str());
    pCharacteristic->notify();
    Serial.println("Notified: " + alertMessage);
    humanActivity++;
  }
  else{
    Serial.println("Device disconnected");
  }
  delay(2000);
}
