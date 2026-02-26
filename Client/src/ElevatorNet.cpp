#include "ElevatorNet.h"

void WifiSetup(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

bool ensureWiFiConnected(const char* ssid, const char* password) {
  if (WiFi.status() == WL_CONNECTED) return true;

  Serial.print("Reconnecting WiFi");
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    return true;
  }

  Serial.println("\nWiFi failed");
  return false;
}

void TimeSetup() {
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "CET-1CEST,M3.5.0/02,M10.5.0/03", 1);
  tzset();
}

String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "N/A";
  }

  char buf[25];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

bool sendToFirebase(const char* firebaseUrl,
                    float floor,
                    float pressure,
                    float pattern,
                    float alarmflag,
                    const String& timestamp) {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skip sending");
    return false;
  }

  HTTPClient http;
  http.begin(firebaseUrl);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["floor"] = floor;
  doc["pressure"] = pressure;
  doc["pattern"] = pattern;
  doc["alarmflag"] = alarmflag;
  doc["timestamp"] = timestamp;

  String payload;
  serializeJson(doc, payload);

  int code = http.POST(payload);

  if (code > 0) {
    Serial.print("FireBase  ");
    Serial.println(payload);
    http.end();
    return true;
  }

  Serial.printf("POST failed: %d\n", code);
  http.end();
  return false;
}
