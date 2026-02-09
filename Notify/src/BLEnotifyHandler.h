#ifndef BLENOTIFYHANDLER_H
#define BLENOTIFYHANDLER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

class BLENotifyHandler {
public:
    BLENotifyHandler();
    void begin();
    void update();

    void setFloor(int value) { floorValue = value; }
    void setPressure(float value) { pressureValue = value; }
    void setMode(uint8_t value) { motionMode = value; }
    void setTime(uint32_t value) { timeValue = value; }

private:
    BLEServer* pServer;
    BLECharacteristic* floorChar;
    BLECharacteristic* pressureChar;
    BLECharacteristic* modeChar;
    BLECharacteristic* timeChar;

    bool deviceConnected;
    bool oldDeviceConnected;

    int floorValue;
    float pressureValue;
    uint8_t motionMode;
    uint32_t timeValue;

    void startAdvertising();

    class MyServerCallbacks : public BLEServerCallbacks {
    public:
        MyServerCallbacks(BLENotifyHandler* handler) : parent(handler) {}
        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;
    private:
        BLENotifyHandler* parent;
    };
};

#endif
