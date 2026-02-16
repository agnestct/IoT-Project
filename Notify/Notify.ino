#include <Arduino.h>
#include "src/BLEnotifyHandler.h"
#include "src/SparkFun_BMP581_Arduino_Library.h"
#include "src/pressuresensor.h"
#include "src/EnvData.h"
#include <cmath>

unsigned long previousMillis = 0; 
const long interval = 50;    
unsigned long startMillis;

const float FLOOR_OFFSETS[] = {0.0f, 42.0f, 83.5f, 118.0f, 153.0f, 187.0f};
#define STATE_IDLE 1
#define STATE_MOVING 2

// -------- Anomaly detection parameters --------
float mu_hat = 99000.0f;        // learned mean
float sigma2_hat = 0.25f;   // variance estimate
int n_samples = 0;

const int Nmax = 20;        // memory length
float gamma_th = 6.0f;     // threshold

const float w = -0.02675;
const float b = 1.88278;
float baseline = 99836;

BLENotifyHandler bleServer;
BMP581 pressureSensor;
EnvData env;



volatile uint32_t lastInterruptTime = 0; 
volatile bool blinkFlag = false;

void IRAM_ATTR updateBaselineInterrupt() {
    uint32_t now = millis();  
    if (now - lastInterruptTime > 200) {  
        baseline = env.pressure;          
        blinkFlag = !blinkFlag;           
        lastInterruptTime = now;         
    }
}


void setup() {
    Serial.begin(115200);
    bleServer.begin();
    ledsetup();
    PressureSensorSetup();
    startMillis = millis();
    delay(1000);
    readSensorData(env.temperature , env.pressure);
    baseline = env.pressure;
    pinMode(SWA_IO, INPUT);
    attachInterrupt(digitalPinToInterrupt(SWA_IO), updateBaselineInterrupt, CHANGE); 
}

void loop() {

   if (blinkFlag) {
        Serial.println("Baseline updated via button press!");
        diodes(0b11111111);
        blinkFlag = !blinkFlag; 
        delay(1000);
        diodes(0b00000000);
    } 

  unsigned long currentMillis = millis();


  readSensorData(env.temperature , env.pressure);
    // env.pressure = 100000 + (rand() % 2000) / 10.0;

  env.floor = w * (env.pressure - baseline) + b;


  double currentpressure = env.pressure;
  patterndection(currentpressure);



  

  // if (currentMillis - startMillis <= 40000) {   
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("Pressure: ");
    Serial.print(env.pressure);
    // Serial.print(" Pa, temperature: ");
    // Serial.println(env.temperature);
    Serial.print(" floor: ");
    Serial.println(env.floor);

    int floorInt = round(env.floor);

    if (floorInt < 0) floorInt = 0;
    if (floorInt > 7) floorInt = 7;

    diodes(1 << floorInt); 
  }
    // }
    env.pattern=1;

  if (env.pattern == STATE_IDLE && env.floor >= 2 && env.floor <= 7) 
  {
      int index = env.floor - 2;
      float current_offset = FLOOR_OFFSETS[index];
      baseline = (env.pressure + current_offset) * 0.05f + baseline * 0.95f;
  }

  delay(100);
    
  bleServer.setFloor(env.floor);
  bleServer.setPressure(env.pressure);
  bleServer.update();
  //BLE update period is defined in BLEnotifyHandler.cpp
}





void patterndection(double x) {
    float lambda;
    if (n_samples < Nmax) {
        lambda = 1.0f / (n_samples + 1);
    } else {
        lambda = 1.0f / Nmax;
    }

    mu_hat = (1.0f - lambda) * mu_hat + lambda * x;
    n_samples++;

    float T = (x - mu_hat) * (x - mu_hat) / sigma2_hat;

    Serial.print("mu=");
    Serial.print(mu_hat);
    Serial.print("  T=");
    Serial.println(T);


    if (T > gamma_th) {
        env.pattern = STATE_MOVING;
        Serial.print("STATE_MOVING ");
//        diodes(0b00000001); // LED ON
    } else {
        env.pattern = STATE_IDLE;
        Serial.print("STATE_IDLE ");
 //       diodes(0b00000000);   // LED OFF
    }
}

