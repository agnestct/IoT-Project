#ifndef BLECLIENTHANDLER_H
#define BLECLIENTHANDLER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <BLERemoteCharacteristic.h>

class BLEClientHandler {
public:
    BLEClientHandler();
    void begin();
    void update();

private:
    // BLE UUIDs
    static BLEUUID serviceUUID;
    static BLEUUID floorUUID;
    static BLEUUID pressureUUID;
    static BLEUUID modeUUID;
    static BLEUUID timeUUID;

    // BLE client state
    static boolean doConnect;
    static boolean connected;
    static boolean doScan;

    static BLERemoteCharacteristic *floorChar;
    static BLERemoteCharacteristic *pressureChar;
    static BLERemoteCharacteristic *modeChar;
    static BLERemoteCharacteristic *timeChar;

    static BLEAdvertisedDevice *myDevice;

    // Data
public:
    static int Floor;
    static float Pressure;
    static uint8_t Mode;
    static uint32_t Time;

private:
    bool connectToServer();

    // Callbacks
    static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                               uint8_t *pData, size_t length, bool isNotify);

    class MyClientCallback : public BLEClientCallbacks {
        void onConnect(BLEClient *pclient) override {}
        void onDisconnect(BLEClient *pclient) override;
    };

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
        void onResult(BLEAdvertisedDevice advertisedDevice) override;
    };
};

#endif
