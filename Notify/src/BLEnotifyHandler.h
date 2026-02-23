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
    void setMessageID(uint32_t value) { messageID = value; }

    bool isConnected() {return deviceConnected;}
    void restartAdvertising() {startAdvertising(); }

    unsigned long connectTime = 0; 
    bool justConnected = false;     
    uint32_t lastClientID = 0;

private:
    BLEServer* pServer;
    BLECharacteristic* floorChar;
    BLECharacteristic* pressureChar;
    BLECharacteristic* modeChar;
    BLECharacteristic* timeChar;
    BLECharacteristic* idChar;

    BLECharacteristic* clientMsgChar;

    bool deviceConnected;
    bool oldDeviceConnected;

    int floorValue;
    float pressureValue;
    uint8_t motionMode;
    uint32_t timeValue;
    uint32_t messageID;

    void startAdvertising();

    class MyServerCallbacks : public BLEServerCallbacks {
    public:
        MyServerCallbacks(BLENotifyHandler* handler) : parent(handler) {}
        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;
    private:
        BLENotifyHandler* parent;
    };
    class MyWriteCallbacks : public BLECharacteristicCallbacks {
    public:
        MyWriteCallbacks(BLENotifyHandler* handler) : parent(handler) {}
        void onWrite(BLECharacteristic* pChar) override;
    private:
        BLENotifyHandler* parent;
    };
};

#endif
