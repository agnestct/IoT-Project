#include <Arduino.h>
#include "src/BLEnotifyHandler.h"
#include "src/SparkFun_BMP581_Arduino_Library.h"
#include "src/pressuresensor.h"
#include "src/EnvData.h"
#include <cmath>

unsigned long previousMillis = 0; 
const long interval = 1000;    


const float w = -0.02637;
const float b = 1.88951;
float baseline = 101013.6;

BLENotifyHandler bleServer;

void setup() {
    Serial.begin(115200);
    bleServer.begin();
    ledsetup();
    // PressureSensorSetup();
}

void loop() {
    EnvData env;

    // readSensorData(env.temperature , env.pressure);

    env.pressure = 100000 + (rand() % 2000) / 10.0;
    env.floor = w * (env.pressure - baseline) + b;

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("Pressure: ");
    Serial.print(env.pressure);
    Serial.print(" Pa, Floor: ");
    Serial.println(env.floor);
  }
 
    bleServer.setFloor(env.floor);
    bleServer.setPressure(env.pressure);
    // bleServer.setMode();

    bleServer.update();
}
