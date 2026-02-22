#include "BLEClientHandler.h"
#include <Arduino.h>

BLEUUID BLEClientHandler::serviceUUID("372208cd-18f9-47f4-b247-bba6aee0792a");
BLEUUID BLEClientHandler::floorUUID("ff62f14e-c79b-40cc-88b8-f810c22436c3");
BLEUUID BLEClientHandler::pressureUUID("7ff73492-7ce2-455e-a2af-5e356af0f039");
BLEUUID BLEClientHandler::modeUUID("8191b89e-8fe6-434f-bce6-759ae6f62f6e");
BLEUUID BLEClientHandler::timeUUID("2d1f529c-5054-43f4-b163-ce6c9fcaa5b0");

boolean BLEClientHandler::doConnect = false;
boolean BLEClientHandler::connected = false;
boolean BLEClientHandler::doScan = false;

BLERemoteCharacteristic* BLEClientHandler::floorChar = nullptr;
BLERemoteCharacteristic* BLEClientHandler::pressureChar = nullptr;
BLERemoteCharacteristic* BLEClientHandler::modeChar = nullptr;
BLERemoteCharacteristic* BLEClientHandler::timeChar = nullptr;

BLEAdvertisedDevice* BLEClientHandler::myDevice = nullptr;

int BLEClientHandler::Floor = 0;
float BLEClientHandler::Pressure = 0.0;
uint8_t BLEClientHandler::Mode = 0;
uint32_t BLEClientHandler::Time = 0;

void BLEClientHandler::notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                                     uint8_t *pData, size_t length, bool isNotify) {
    BLEUUID uuid = pBLERemoteCharacteristic->getUUID();

    if (uuid.equals(floorUUID)) {
        memcpy(&Floor, pData, sizeof(Floor));
    } 
    else if (uuid.equals(pressureUUID)) {
        memcpy(&Pressure, pData, sizeof(Pressure));
    } 
    else if (uuid.equals(modeUUID)) {
        Mode = pData[0];
    } 
    else if (uuid.equals(timeUUID)) {
        memcpy(&Time, pData, sizeof(Time));
    }

}

void BLEClientHandler::MyClientCallback::onDisconnect(BLEClient *pclient) {
    BLEClientHandler::connected = false;
    Serial.println("Disconnected from server");
}


void BLEClientHandler::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;
        Serial.println("Found our device! Connecting...");
    }
}


bool BLEClientHandler::connectToServer() {
    BLEClient *pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(myDevice);
    pClient->setMTU(517);

    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) return false;

    floorChar = pRemoteService->getCharacteristic(floorUUID);
    pressureChar = pRemoteService->getCharacteristic(pressureUUID);
    modeChar = pRemoteService->getCharacteristic(modeUUID);
    timeChar = pRemoteService->getCharacteristic(timeUUID);

    if (floorChar && floorChar->canNotify()) floorChar->registerForNotify(notifyCallback);
    if (pressureChar && pressureChar->canNotify()) pressureChar->registerForNotify(notifyCallback);
    if (modeChar && modeChar->canNotify()) modeChar->registerForNotify(notifyCallback);
    if (timeChar && timeChar->canNotify()) timeChar->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

BLEClientHandler::BLEClientHandler() {}

void BLEClientHandler::begin() {
    Serial.begin(115200);
    BLEDevice::init("");

    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);

    Serial.println("Scanning for BLE server...");
}

void BLEClientHandler::update() {
    static unsigned long lastPrintTime = 0;
    unsigned long now = millis();

    if (doConnect) {
        if (connectToServer()) {
            Serial.println("Connected to BLE server.");
        } else {
            Serial.println("Failed to connect to server.");
        }
        doConnect = false;
    }

    if (!connected && doScan) {
        BLEDevice::getScan()->start(0);
    }

    if (connected && (now - lastPrintTime >=300)) {
        lastPrintTime = now;
        Serial.print("BLE  ");
        Serial.print("Floor: "); Serial.print(Floor);
        Serial.print("\tPressure: "); Serial.print(Pressure);
        Serial.print("\tMotion Mode: "); Serial.print(Mode);
        Serial.print("\tAlarmFlag: "); Serial.println(Time);
    }
}
