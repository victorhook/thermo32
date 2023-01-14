#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H


#define WIFI_ACCESS_POINT_SSID "THERMO32"
#define WIFI_ACCESS_POINT_PASSWORD "THERMO32"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

class WiFiHandler {
    public:
        WiFiHandler();
        bool isConnectedToWifi();
        void startAsAccessPoint();
        bool connectToWiFi(const int timeout_s);
};


#endif /* WIFI_HANDLER_H */
