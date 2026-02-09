#include "src/ElevatorNet.h"
#include "src/EnvData.h"
#include "src/EduroamWiFi.h"
#include "src/SparkFun_BMP581_Arduino_Library.h"
#include <cmath>


// Create a new sensor object
BMP581 pressureSensor;

// I2C address selection
uint8_t i2cAddress = BMP581_I2C_ADDRESS_DEFAULT; // 0x47
//uint8_t i2cAddress = BMP581_I2C_ADDRESS_SECONDARY; // 0x46

#define EAP_IDENTITY "wzeng@kth.se"
#define EAP_USERNAME "wzeng@kth.se"
#define EAP_PASSWORD "Mx97yVpg2kZ"

const char* SSID = "eduroam";
const char* HOST = "httpbin.org";

EduroamWiFi wifi(SSID, HOST, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

// const char* ssid = "HUAWEI-A1C6DL";
// const char* password = "zws@8888"; //using this if you want use the wihi home

const char* firebaseUrl =
  "https://elevatormonitor-3e8cd-default-rtdb.europe-west1.firebasedatabase.app/elevator.json";
  //using https://elevatormonitor-3e8cd.web.app/ as the real-time elevator monitoring dashboard

const float w = -0.02491;
const float b = 1.95941;
float baseline = 101089;

void setup() {
  Serial.begin(115200);
  wifi.connect();
  // WifiSetup(ssid, password);
  TimeSetup();
  ledsetup();
  PressureSensorSetup();
}

void loop() {
  
  wifi.maintainConnection();
  // EnvData env = randmEnvData();
  EnvData env;

  readSensorData(env.temperature , env.pressure);

  // if(current pattern = idle){
  //   if(current floor = 2 floor){
  //   baseline = env.pressure ;
  // }}

  // baseline = 0.95 * baseline + 0.05 * env.pressure;

  // float newpressure = env.pressure - currentpressure ;

  env.floor = std::round(w * (env.pressure - baseline) + b);

  sendToFirebase(
    firebaseUrl,
    env.floor,
    env.pressure,
    env.temperature,
    getTimeString()
  );


  // if (ensureWiFiConnected(ssid, password)) {
  // EnvData env = randmEnvData();
  // sendToFirebase(
  //   firebaseUrl,
  //   env.floor,
  //   env.pressure,
  //   env.temperature,
  //   getTimeString()
  // );
  // } //uncomment this to enable wifi home

  delay(500);
}


void PressureSensorSetup(){
    Serial.println("BMP581 Example1 begin!");
    // Serial.println(BMP5_OK);
    // delay(1000);

    // Initialize the I2C library
    Wire.begin(2,1,10000);

    // Check if sensor is connected and initialize
    // Address is optional (defaults to 0x47)
    while(pressureSensor.beginI2C(i2cAddress) != BMP5_OK)
    {
        // Not connected, inform user
        Serial.println("Error: BMP581 not connected, check wiring and I2C address!");

        // Wait a bit to see if connection is established
        delay(1000);
    }

    Serial.println("BMP581 connected!");
}

bool readSensorData(float &temperature, float &pressure) {
    bmp5_sensor_data data = {0,0};
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

