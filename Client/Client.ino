#include <Arduino.h>
#include "src/BLEClientHandler.h"
#include "src/EduroamWiFi.h"
#include "src/ElevatorNet.h"
#include "src/EnvData.h"

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // 1000ms
float lastFloor = -1; 
float lastFlag = -1; 

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

bool debug = true;

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

unsigned long lastCheck = 0;
const unsigned long interval = 300;

#define QUEUE_SIZE 8

EnvData queue[QUEUE_SIZE];
int head = 0; 
int tail = 0; 
int count = 0; 

void enqueue(const EnvData& data) {
    queue[head] = data;
    head = (head + 1) % QUEUE_SIZE;
    if (count < QUEUE_SIZE) {
        count++;
    } else {
        tail = (tail + 1) % QUEUE_SIZE;
    }
}

bool dequeue(EnvData &data) {
    if (count == 0) return false;
    data = queue[tail];
    tail = (tail + 1) % QUEUE_SIZE;
    count--;
    return true;
}


void loop() {

    if (millis() - lastCheck >= interval) {
        lastCheck = millis();

        bleClient.update();

        Serial.print("BLE  ");
        Serial.print("Floor: "); Serial.print(bleClient.Floor);
        Serial.print("\tPressure: "); Serial.print(bleClient.Pressure);
        Serial.print("\tMotion Mode: "); Serial.print(bleClient.Mode);
        Serial.print("\tAlarmFlag: "); Serial.print(bleClient.Time);
        Serial.print("\tMessageID: "); Serial.println(bleClient.MessageID);

        env.pressure = bleClient.Pressure;
        env.floor = bleClient.Floor;
        env.pattern = bleClient.Mode;
        AlarmFlag = bleClient.Time;

        if (env.floor != lastFloor || AlarmFlag != lastFlag) {
            lastFloor = env.floor;
            lastFlag = AlarmFlag;

            EnvData data;
            data.floor = env.floor;
            data.pressure = env.pressure;
            data.pattern = env.pattern;
            data.alarmFlag = AlarmFlag;
            data.timestamp = getTimeString();

            enqueue(data);

            Serial.println("Data enqueued");
        }
    }

    if (ensureWiFiConnected(ssid, password)) {
        EnvData toSend;
        while (dequeue(toSend)) {
            diodes(0b11111111);

            sendToFirebase(
                firebaseUrl,
                toSend.floor,
                toSend.pressure,
                toSend.pattern,
                toSend.alarmFlag,
                toSend.timestamp
            );

            Serial.print("WIFI MessageID: "); Serial.println(bleClient.MessageID);

            diodes(0b00000000);
        }
    }
}
