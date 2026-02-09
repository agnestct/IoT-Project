#ifndef ENVDATA_H
#define ENVDATA_H

#include <Arduino.h>
#include <math.h>



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

#endif
