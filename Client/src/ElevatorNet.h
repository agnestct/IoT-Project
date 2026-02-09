#ifndef ELEVATOR_NET_H
#define ELEVATOR_NET_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

void WifiSetup(const char* ssid, const char* password);
void TimeSetup();

bool ensureWiFiConnected(const char* ssid, const char* password);

String getTimeString();

bool sendToFirebase(const char* firebaseUrl,
                    float floor,
                    float pressure,
                    float temperature,
                    const String& timestamp);

#endif
