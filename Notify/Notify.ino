#include <Arduino.h>
#include <cmath>

#include "src/BLEnotifyHandler.h"
#include "src/SparkFun_BMP581_Arduino_Library.h"
#include "src/pressuresensor.h"
#include "src/EnvData.h"
#include "src/MovementTracker.h"
#include "src/MyICM20948.h"


// ------------------ Global variables ------------------
unsigned long previousMillis = 0; 
const long interval = 500;    
unsigned long startMillis;

const float FLOOR_OFFSETS[] = {0.0f, 42.0f, 83.5f, 118.0f, 153.0f, 187.0f};
#define STATE_IDLE 1
#define STATE_MOVING 2

// -------- Anomaly detection parameters --------
float mu_hat = 99000.0f;        // learned mean
float sigma2_hat = 0.25f;       // variance estimate
int n_samples = 0;

const int Nmax = 20;        // memory length
float gamma_th = 6.0f;      // threshold

const float w = -0.02675;
const float b = 1.88278;
float baseline = 99836;

float T=0;

BLENotifyHandler bleServer;
BMP581 pressureSensor;
MovementTracker movementTracker;

TwoWire I2C_IMU = TwoWire(1); 

MyICM20948 imu(I2C_IMU, 1);

EnvData env;

int floorInt = 0;

volatile uint32_t lastInterruptTime = 0; 
volatile bool blinkFlag = false;

int startFloor=2;
int endFloor=2;

int AlarmFlag=0;

int lastFloor = -1;
int lastAlarmFlag = -1;

#define QUEUE_SIZE 60

struct BLEPacket {
    int mode;
    int floor;
    float pressure;
    int alarmFlag;
    uint32_t msgID;
};

BLEPacket dataQueue[QUEUE_SIZE];
int stackTop = -1;

uint32_t messageIDCounter = 1;

int head = 0;   
int tail = 0;   
int count = 0;  





// ------------------ Interrupt ------------------
void IRAM_ATTR updateBaselineInterrupt() {
    uint32_t now = millis();  
    if (now - lastInterruptTime > 200) {  
        baseline = env.pressure;          // update baseline
        blinkFlag = !blinkFlag;           // toggle blink flag
        lastInterruptTime = now;         
    }
}




// ------------------ Helper functions ------------------
void handleBlinkFlag() {
    if (blinkFlag) {
        Serial.println("Baseline updated via button press!");
        diodes(0b11111111);
        blinkFlag = !blinkFlag; 
        delay(1000);
        diodes(0b00000000);
    } 
}

void updateEnvData() {
    readSensorData(env.temperature , env.pressure);
    env.floor = w * (env.pressure - baseline) + b;
}

void printEnvData() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        Serial.print("Pressure: "); Serial.print(env.pressure);
        Serial.print(" | Floor: "); Serial.print(env.floor);
        Serial.print(" | Baseline: "); Serial.print(baseline);
        Serial.print(" | mu: "); Serial.print(mu_hat);
        Serial.print(" | T: "); Serial.print(T);
        Serial.print(" | State: ");
        Serial.println(env.pattern == STATE_MOVING ? "MOVING" : "IDLE");
        imu.printScaled();
        Serial.print("Trajectory: ");
        Serial.print(startFloor);
        Serial.print(" -> ");
        Serial.println(endFloor);


        float magSquared = sqrt(
          imu.magX()*imu.magX() +
          imu.magY()*imu.magY() +
          imu.magZ()*imu.magZ()
      );
        float accSquared = sqrt(
            imu.accX()*imu.accX() +
            imu.accY()*imu.accY() +
            imu.accZ()*imu.accZ()
        );

        // float gyrSquared = sqrt(
        //     imu.gyrX()*imu.gyrX() +
        //     imu.gyrY()*imu.gyrY() +
        //     imu.gyrZ()*imu.gyrZ()
        // );


        Serial.print("ACC Norm: ");
        Serial.print(accSquared-980, 6);   
        // Serial.print("  GYR Norm: ");
        // Serial.print(gyrSquared, 6);
        Serial.print("  MAG Norm: ");
        Serial.println(magSquared, 6);
        Serial.println("****************");


    }
}


void updateFloor() {
    floorInt = round(env.floor);
    if (floorInt < 0) floorInt = 0;
    if (floorInt > 7) floorInt = 7;

    diodes(1 << floorInt); 

    // Auto-update baseline
    if (env.pattern == STATE_IDLE && floorInt >= 2 && floorInt <= 7) {
        int index = floorInt - 2;
        float current_offset = FLOOR_OFFSETS[index];
        baseline = (env.pressure + current_offset) * 0.05f + baseline * 0.95f;
        //Serial.println(" baseline updated ");
    }
}

int handleMovement() {
    int traj = movementTracker.trackMovement(env.pattern, floorInt); 
    if (traj != -1) {
        startFloor = traj & 0x0F;
        endFloor = (traj >> 4) & 0x0F;
    }
    return traj; 
}

// ------------------ BLE update function ------------------


// void updateBLE(BLENotifyHandler &bleServer, int traj, int floorInt, float pressure, int AlarmFlag) {
//     if (bleServer.isConnected()) {
//         bleServer.setMode(traj);
//         bleServer.setFloor(floorInt);
//         bleServer.setPressure(pressure);
//         bleServer.setTime(AlarmFlag);
//         bleServer.update();

//         Serial.println(" -> BLE connected, data sent.");
//     } else {
//         bleServer.restartAdvertising();
//         Serial.println(" -> BLE disconnected, cannot send data.");
//     }
// }

bool pushData(int mode, int floor, float pressure, int alarmFlag) {
    if (count >= QUEUE_SIZE) {
        Serial.print("[QUEUE] Overflow. Removed MsgID: ");
        Serial.println(dataQueue[head].msgID);

        head = (head + 1) % QUEUE_SIZE;
        count--;
    }

    uint32_t newMsgID = messageIDCounter++;
    dataQueue[tail] = {mode, floor, pressure, alarmFlag, newMsgID};
    tail = (tail + 1) % QUEUE_SIZE;
    count++;

    Serial.print("[QUEUE PUSH] MsgID: ");
    Serial.println(newMsgID);

    return true;
}

void updateBLE(BLENotifyHandler &bleServer,
               int traj,
               int floorInt,
               float pressure,
               int AlarmFlag) {

    static bool wasConnected = false;
    bool isNowConnected = bleServer.isConnected();

    pushData(traj, floorInt, pressure, AlarmFlag);

    if (isNowConnected) {
        if (!wasConnected) {
            Serial.println("[BLE] Connected.");
            wasConnected = true;
        }

        if (count > 0) {
            BLEPacket &packet = dataQueue[head];

            bleServer.setMode(packet.mode);
            bleServer.setFloor(packet.floor);
            bleServer.setPressure(packet.pressure);
            bleServer.setTime(packet.alarmFlag);
            bleServer.setMessageID(packet.msgID); 
            bleServer.update();

            Serial.print("[QUEUE SEND] MsgID: ");
            Serial.println(packet.msgID);

            if (bleServer.lastClientID == packet.msgID) {
                head = (head + 1) % QUEUE_SIZE;
                count--;
                bleServer.lastClientID = 0;

                Serial.print("[QUEUE ACK] MsgID: ");
                Serial.println(packet.msgID);
            }
        }

    } else {
        if (wasConnected) Serial.println("[BLE] Disconnected.");
        wasConnected = false;
        bleServer.restartAdvertising();
    }
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

    T = (x - mu_hat) * (x - mu_hat) / sigma2_hat;

    // Serial.print(" mu=");
    // Serial.print(mu_hat);
    // Serial.print(" T=");
    // Serial.println(T);

    if (T > gamma_th) {
        env.pattern = STATE_MOVING;
        // Serial.print(" STATE_MOVING ");
    } else {
        env.pattern = STATE_IDLE;
        // Serial.print(" STATE_IDLE ");
    }
}




void setup() {
    Serial.begin(115200);
    bleServer.begin();
    ledsetup();
    PressureSensorSetup();
    delay(50); 
    imu.begin(4, 3, 10000);
    delay(3000);
    readSensorData(env.temperature , env.pressure);
    baseline = env.pressure;
    pinMode(SWA_IO, INPUT);
    attachInterrupt(digitalPinToInterrupt(SWA_IO), updateBaselineInterrupt, CHANGE); 
    AlarmFlag=0;
    
    startMillis = millis();
}


unsigned long previousMillisforBLE = 0;
const unsigned long intervalforBLE = 500;      

unsigned long previousMillisforAlarm = 0;
const unsigned long intervalForAlarm = 800;    

void loop() {
    handleBlinkFlag();
    unsigned long currentMillis = millis();



    if (currentMillis - previousMillisforBLE >= intervalforBLE) {
        AlarmFlag++;
        updateEnvData();
        patterndection(env.pressure);
        updateFloor();
        imu.update();
        // imu.printScaled();
        int traj = handleMovement();  
        // printEnvData();
        previousMillisforBLE = currentMillis;
        updateBLE(bleServer, traj, floorInt, env.pressure, AlarmFlag);


        if (bleServer.lastClientID > 0) {  
            bleServer.lastClientID = 0;    
        }

    }

    // if (currentMillis - previousMillisforAlarm >= intervalForAlarm) {
    //     previousMillisforAlarm = currentMillis;
    //     ;  
    // }

}