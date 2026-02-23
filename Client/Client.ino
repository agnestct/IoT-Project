#include <Arduino.h>
#include "src/BLEClientHandler.h"
#include "src/EduroamWiFi.h"
#include "src/ElevatorNet.h"
#include "src/EnvData.h"

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // 1000ms
float lastFloor = -1; 
float lastPattern = -1; 

#define EAP_IDENTITY "wzeng@kth.se"
#define EAP_USERNAME "wzeng@kth.se"
#define EAP_PASSWORD "Mx97yVpg2kZ"

const char* SSID = "eduroam";
const char* HOST = "httpbin.org";

EduroamWiFi wifi(SSID, HOST, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

const char* ssid = "HUAWEI-A1C6DL";
const char* password = "zws@8888"; //using this if you want use the wihi home

const char* firebaseUrl =
  "https://elevatormonitor-3e8cd-default-rtdb.europe-west1.firebasedatabase.app/elevator.json";
  //using https://elevatormonitor-3e8cd.web.app/ as the real-time elevator monitoring dashboard

BLEClientHandler bleClient;

bool debug = false;

unsigned long previousMillis = 0;

EnvData env;

int AlarmFlag=0;


void setup() {
    Serial.begin(115200);


    if (debug) {
        WifiSetup(ssid, password);     
    } else {
        wifi.connect();  
    }

    TimeSetup();
    ledsetup();
    bleClient.begin();
    diodes(0b00000000);
}

void loop() {

    
    bleClient.update();
    // wifi.maintainConnection();


    env.pressure = bleClient.Pressure;
    env.floor = bleClient.Floor;
    env.pattern = bleClient.Mode;
    AlarmFlag = bleClient.Time;

   if(ensureWiFiConnected(ssid, password)){

        if (env.floor != lastFloor||) {
            lastFloor = env.floor;       
            lastPattern = env.pattern;  

            diodes(0b11111111);    
            sendToFirebase(
                firebaseUrl,
                env.floor,
                env.pressure,
                env.pattern,
                AlarmFlag,
                getTimeString() 
            );
            diodes(0b00000000);
        }
    }
   
}
