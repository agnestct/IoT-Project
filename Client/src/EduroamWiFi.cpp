#include "EduroamWiFi.h"
#include <WiFi.h>

EduroamWiFi::EduroamWiFi(const char* _ssid,
                         const char* _host,
                         const char* _identity,
                         const char* _username,
                         const char* _password,
                         int timeout) {
    ssid = _ssid;
    host = _host;
    identity = _identity;
    username = _username;
    password = _password;
    timeoutSeconds = timeout;
}

bool EduroamWiFi::connect() {
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(ssid);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, WPA2_AUTH_PEAP, identity, username, password);

    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        counter++;
        if (counter >= timeoutSeconds * 2) {
            Serial.println("\nTimeout, restarting...");
            ESP.restart();
        }
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void EduroamWiFi::maintainConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Lost connection, reconnecting...");
        connect();
    }
}

void EduroamWiFi::httpGet(const char* path) {
    WiFiClient client;
    if (!client.connect(host, 80)) {
        Serial.println("Connection failed");
        return;
    }

    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: ESP32\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") break;
    }

    String response = client.readStringUntil('\n');
    Serial.println("HTTP Response:");
    Serial.println(response);
}
