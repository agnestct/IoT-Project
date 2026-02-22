#ifndef MY_ICM20948_H
#define MY_ICM20948_H

#include <Wire.h>
#include "src/ICM_20948.h"

class MyICM20948 {
public:
    MyICM20948(TwoWire &wirePort = Wire, uint8_t ad0 = 1);
    bool begin(uint8_t sda = 4, uint8_t scl = 3, uint32_t clock = 400000);
    void update();
    bool dataReady();
    
    // 获取缩放后的数据
    float accX(), accY(), accZ();
    float gyrX(), gyrY(), gyrZ();
    float magX(), magY(), magZ();
    float temp();

    // 打印数据
    void printRaw();
    void printScaled();

private:
    ICM_20948_I2C _sensor;
    TwoWire *_wire;
    uint8_t _ad0;

    void printPaddedInt16b(int16_t val);
    void printFormattedFloat(float val, uint8_t leading, uint8_t decimals);
};

#endif
