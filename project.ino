#include "src/ElevatorNet.h"
#include "src/EnvData.h"


const char* ssid = "HUAWEI-A1C6DL";
const char* password = "zws@8888";
const char* firebaseUrl =
  "https://elevatormonitor-3e8cd-default-rtdb.europe-west1.firebasedatabase.app/elevator.json";
  //using https://elevatormonitor-3e8cd.web.app/ as the real-time elevator monitoring dashboard


void setup() {
  Serial.begin(115200);
  WifiSetup(ssid, password);
  TimeSetup();
}

void loop() {
  if (ensureWiFiConnected(ssid, password)) {

    EnvData env = randmEnvData();

    sendToFirebase(
      firebaseUrl,
      env.floor,
      env.pressure,
      env.temperature,
      getTimeString()
    );
  }

  delay(5000);
}


