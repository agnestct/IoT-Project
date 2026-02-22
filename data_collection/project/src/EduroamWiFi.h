#ifndef EDUROAM_WIFI_H
#define EDUROAM_WIFI_H

#include <WiFi.h>

class EduroamWiFi {
private:
    int timeoutSeconds;
    const char* ssid;
    const char* host;
    const char* identity;
    const char* username;
    const char* password;

public:
    EduroamWiFi(const char* _ssid,
                const char* _host,
                const char* _identity,
                const char* _username,
                const char* _password,
                int timeout = 30);

    bool connect();                  
    void maintainConnection();       
    void httpGet(const char* path);  
};

#endif
