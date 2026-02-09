#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "SparkFun_BMP581_Arduino_Library.h"
#include <Wire.h>

extern BMP581 pressureSensor;

extern uint8_t i2cAddress;

void PressureSensorSetup();
bool readSensorData(float &temperature, float &pressure);

#endif
