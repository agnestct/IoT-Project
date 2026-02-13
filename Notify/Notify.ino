#include <Arduino.h>
#include "src/BLEnotifyHandler.h"
#include "src/SparkFun_BMP581_Arduino_Library.h"
#include "src/pressuresensor.h"
#include "src/EnvData.h"
#include <cmath>

unsigned long previousMillis = 0; 
const long interval = 50;    
unsigned long startMillis;

const float FLOOR_OFFSETS[] = {0.0f, 40.0f, 80.0f, 120.0f, 160.0f};
#define STATE_IDLE 1


const float w = -0.02675;
const float b = 1.88278;
float baseline = 99836;

BLENotifyHandler bleServer;
BMP581 pressureSensor;


void setup() {
    Serial.begin(115200);
    bleServer.begin();
    ledsetup();
    PressureSensorSetup();
    startMillis = millis();
    delay(1000);
}

void loop() {
  EnvData env;

  readSensorData(env.temperature , env.pressure);
    // env.pressure = 100000 + (rand() % 2000) / 10.0;
  env.floor = w * (env.pressure - baseline) + b;

  unsigned long currentMillis = millis();

    // if (currentMillis - startMillis <= 40000) {   
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("Pressure: ");
    Serial.print(env.pressure);
    Serial.print(" Pa, temperature: ");
    Serial.println(env.temperature);
  }
    // }

  // if (env.pattern == STATE_IDLE && env.floor >= 2 && env.floor <= 6) 
  // {
  //     int index = env.floor - 2;
  //     float current_offset = FLOOR_OFFSETS[index];
      
  //     baseline = (env.pressure - current_offset) * 0.01f + baseline * 0.99f;
  // }



    
  bleServer.setFloor(env.floor);
  bleServer.setPressure(env.pressure);
  // bleServer.setMode();
  bleServer.update();
  //BLE update period is defined in BLEnotifyHandler.cpp
}
