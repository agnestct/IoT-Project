#include <Arduino.h>
#include "src/BLEnotifyHandler.h"

BLENotifyHandler bleServer;

void setup() {
    Serial.begin(115200);
    bleServer.begin();
}

void loop() {
    bleServer.update();
}
