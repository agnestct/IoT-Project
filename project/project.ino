#include "src/ElevatorNet.h"
#include "src/EnvData.h"
#include "EduroamWiFi.h"

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


void setup() {
  Serial.begin(115200);
  wifi.connect();
  // WifiSetup(ssid, password);
  TimeSetup();
}

void loop() {
  
  wifi.maintainConnection();
  EnvData env = randmEnvData();
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

  delay(5000);
}


