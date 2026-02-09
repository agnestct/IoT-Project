#ifndef ENVDATA_H
#define ENVDATA_H

#include <Arduino.h>
#include <math.h>
#include "dsplp_io.h"  



struct EnvData {
  float pressure;     // Pa
  float temperature;  // °C
  float height;       // m
  int   floor;        
};

EnvData randmEnvData() {
  const float P0 = 101325.0; 

  EnvData data;
  data.pressure = random(101190, 101325);      // Pa
  data.temperature = random(200, 300) / 10.0;  // °C

  data.height = 44330.0 * (1.0 - pow(data.pressure / P0, 0.1903));
  data.floor = (int)floorf(data.height / 3.0); 

  return data;
}


// -------------------------------------------------------------
// LED shift-register helper
// -------------------------------------------------------------
void diodes(uint8_t leds) {
    for (int led = 0; led < 8; led++) {
        digitalWrite(LED_SHCP_IO, LOW);
        (leds & (1 << led)) ? digitalWrite(LED_SDA_IO, HIGH)
                           : digitalWrite(LED_SDA_IO, LOW);
        delayMicroseconds(1);
        digitalWrite(LED_SHCP_IO, HIGH);
        delayMicroseconds(1);
    }

    digitalWrite(LED_SHCP_IO, LOW);
    digitalWrite(LED_SDA_IO, LOW);
    digitalWrite(LED_STCP_IO, HIGH);
    delayMicroseconds(1);
    digitalWrite(LED_SHCP_IO, HIGH);
    digitalWrite(LED_STCP_IO, LOW);
}


void ledsetup(){
  pinMode(LED_SDA_IO,  OUTPUT);
  pinMode(LED_SHCP_IO, OUTPUT);
  pinMode(LED_STCP_IO, OUTPUT);

  diodes(0b00000000);  // Turn off all LEDs
}

#endif
