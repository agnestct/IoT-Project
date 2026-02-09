#include "pressuresensor.h"
#include <Arduino.h>

uint8_t i2cAddress = BMP581_I2C_ADDRESS_DEFAULT; // 0x47
//uint8_t i2cAddress = BMP581_I2C_ADDRESS_SECONDARY; // 0x46

BMP581 pressureSensor;

void PressureSensorSetup() {
    Serial.println("BMP581 Example1 begin!");

    Wire.begin(2, 1, 10000);

    while (pressureSensor.beginI2C(i2cAddress) != BMP5_OK) {
        Serial.println("Error: BMP581 not connected, check wiring and I2C address!");
        delay(1000);
    }

    Serial.println("BMP581 connected!");
}

bool readSensorData(float &temperature, float &pressure) {
    bmp5_sensor_data data = {0, 0};
    int8_t err = pressureSensor.getSensorData(&data);

    if (err == BMP5_OK) {
        temperature = data.temperature;
        pressure = data.pressure;
        return true;
    } else {
        Serial.print("Error getting data from sensor! Error code: ");
        Serial.println(err);
        return false;
    }
}
