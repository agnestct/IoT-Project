#include "BLEnotifyHandler.h"
#include <Arduino.h>

unsigned long previousMillisforble = 0;
const unsigned long intervalforble = 1000; //BLE update period

unsigned long connectTime = 0;
const unsigned long waitAfterConnect = 2000; 
bool justConnected = false;

#define SERVICE_UUID "372208cd-18f9-47f4-b247-bba6aee0792a"
#define FLOOR_CHAR_UUID "ff62f14e-c79b-40cc-88b8-f810c22436c3"
#define PRESSURE_CHAR_UUID "7ff73492-7ce2-455e-a2af-5e356af0f039"
#define MODE_CHAR_UUID "8191b89e-8fe6-434f-bce6-759ae6f62f6e"
#define TIME_CHAR_UUID "2d1f529c-5054-43f4-b163-ce6c9fcaa5b0"

BLENotifyHandler::BLENotifyHandler()
    : pServer(nullptr),
      floorChar(nullptr),
      pressureChar(nullptr),
      modeChar(nullptr),
      timeChar(nullptr),
      deviceConnected(false),
      oldDeviceConnected(false),
      floorValue(3),
      pressureValue(101325.0),
      motionMode(1),
      timeValue(0) {}

void BLENotifyHandler::begin() {
    BLEDevice::init("ESP32");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));

    BLEService* pService = pServer->createService(SERVICE_UUID);

    floorChar = pService->createCharacteristic(FLOOR_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    floorChar->addDescriptor(new BLE2902());

    pressureChar = pService->createCharacteristic(PRESSURE_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    pressureChar->addDescriptor(new BLE2902());

    modeChar = pService->createCharacteristic(MODE_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    modeChar->addDescriptor(new BLE2902());

    timeChar = pService->createCharacteristic(TIME_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    timeChar->addDescriptor(new BLE2902());

    pService->start();
    startAdvertising();

    Serial.println("Waiting for client connection...");
}

void BLENotifyHandler::update() {
    unsigned long currentMillisforble = millis();

    if (deviceConnected) {

        // 刚连接，等待客户端初始化
        if (justConnected) {
            if (currentMillisforble - connectTime < waitAfterConnect) {
                return;  // 等待
            } else {
                justConnected = false; // 等够时间了，清标记
            }
        }

        if (currentMillisforble - previousMillisforble >= intervalforble) {
            previousMillisforble = currentMillisforble;

            floorChar->setValue((uint8_t*)&floorValue, sizeof(floorValue));
            floorChar->notify();

            pressureChar->setValue((uint8_t*)&pressureValue, sizeof(pressureValue));
            pressureChar->notify();

            modeChar->setValue((uint8_t*)&motionMode, sizeof(motionMode));
            modeChar->notify();

            timeChar->setValue((uint8_t*)&timeValue, sizeof(timeValue));
            timeChar->notify();
        }
    }

    if (!deviceConnected && oldDeviceConnected) {
        startAdvertising();
        oldDeviceConnected = deviceConnected;
    }

    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}


void BLENotifyHandler::startAdvertising() {
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    BLEDevice::startAdvertising();
}

void BLENotifyHandler::MyServerCallbacks::onConnect(BLEServer* pServer) {
    parent->deviceConnected = true;
    parent->connectTime = millis();
    parent->justConnected = true; 
}

void BLENotifyHandler::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    parent->deviceConnected = false;
}
