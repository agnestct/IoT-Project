#include "MyICM20948.h"
#include <Arduino.h>

MyICM20948::MyICM20948(TwoWire &wirePort, uint8_t ad0) : _wire(&wirePort), _ad0(ad0) {}

bool MyICM20948::begin(uint8_t sda, uint8_t scl, uint32_t clock) {
    _wire->begin(sda, scl);
    _wire->setClock(clock);

    bool initialized = false;
    while (!initialized) {
        _sensor.begin(*_wire, _ad0);
        Serial.print(F("Initialization returned: "));
        Serial.println(_sensor.statusString());
        if (_sensor.status != ICM_20948_Stat_Ok) {
            Serial.println("Retrying...");
            delay(500);
        } else {
            initialized = true;
        }
    }
    return initialized;
}

void MyICM20948::update() {
    if (_sensor.dataReady()) {
        _sensor.getAGMT();
    }
}

bool MyICM20948::dataReady() {
    return _sensor.dataReady();
}

float MyICM20948::accX() { return _sensor.accX(); }
float MyICM20948::accY() { return _sensor.accY(); }
float MyICM20948::accZ() { return _sensor.accZ(); }
float MyICM20948::gyrX() { return _sensor.gyrX(); }
float MyICM20948::gyrY() { return _sensor.gyrY(); }
float MyICM20948::gyrZ() { return _sensor.gyrZ(); }
float MyICM20948::magX() { return _sensor.magX(); }
float MyICM20948::magY() { return _sensor.magY(); }
float MyICM20948::magZ() { return _sensor.magZ(); }
float MyICM20948::temp() { return _sensor.temp(); }

void MyICM20948::printRaw() {
    ICM_20948_AGMT_t agmt = _sensor.agmt;
    Serial.print("RAW. Acc [ ");
    printPaddedInt16b(agmt.acc.axes.x);
    Serial.print(", ");
    printPaddedInt16b(agmt.acc.axes.y);
    Serial.print(", ");
    printPaddedInt16b(agmt.acc.axes.z);
    Serial.print(" ], Gyr [ ");
    printPaddedInt16b(agmt.gyr.axes.x);
    Serial.print(", ");
    printPaddedInt16b(agmt.gyr.axes.y);
    Serial.print(", ");
    printPaddedInt16b(agmt.gyr.axes.z);
    Serial.print(" ], Mag [ ");
    printPaddedInt16b(agmt.mag.axes.x);
    Serial.print(", ");
    printPaddedInt16b(agmt.mag.axes.y);
    Serial.print(", ");
    printPaddedInt16b(agmt.mag.axes.z);
    Serial.print(" ], Tmp [ ");
    printPaddedInt16b(agmt.tmp.val);
    Serial.println(" ]");
}

void MyICM20948::printScaled() {
    Serial.print("Scaled. Acc (mg) [ ");
    printFormattedFloat(accX(), 5, 2);
    Serial.print(", ");
    printFormattedFloat(accY(), 5, 2);
    Serial.print(", ");
    printFormattedFloat(accZ(), 5, 2);
    // Serial.print(" ], Gyr (DPS) [ ");
    // printFormattedFloat(gyrX(), 5, 2);
    // Serial.print(", ");
    // printFormattedFloat(gyrY(), 5, 2);
    // Serial.print(", ");
    // printFormattedFloat(gyrZ(), 5, 2);
    Serial.print(" ], Mag (uT) [ ");
    printFormattedFloat(magX(), 5, 2);
    Serial.print(", ");
    printFormattedFloat(magY(), 5, 2);
    Serial.print(", ");
    printFormattedFloat(magZ(), 5, 2);
    Serial.print(" ], Tmp (C) [ ");
    printFormattedFloat(temp(), 5, 2);
    Serial.println(" ]");
}

void MyICM20948::printPaddedInt16b(int16_t val) {
    if (val > 0) {
        Serial.print(" ");
        if (val < 10000) Serial.print("0");
        if (val < 1000) Serial.print("0");
        if (val < 100) Serial.print("0");
        if (val < 10) Serial.print("0");
    } else {
        Serial.print("-");
        int16_t absVal = abs(val);
        if (absVal < 10000) Serial.print("0");
        if (absVal < 1000) Serial.print("0");
        if (absVal < 100) Serial.print("0");
        if (absVal < 10) Serial.print("0");
    }
    Serial.print(abs(val));
}

void MyICM20948::printFormattedFloat(float val, uint8_t leading, uint8_t decimals) {
    if (val < 0) Serial.print("-");
    else Serial.print(" ");
    Serial.print(abs(val), decimals);
}
