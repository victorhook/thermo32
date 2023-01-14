#include "wifi_handler.h"
#include "WiFi.h"


WiFiHandler::WiFiHandler() {

}

bool WiFiHandler::isConnectedToWifi() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiHandler::startAsAccessPoint() {
    WiFi.softAP(WIFI_ACCESS_POINT_SSID, WIFI_ACCESS_POINT_PASSWORD);
}

bool WiFiHandler::connectToWiFi(const int timeout_s) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    unsigned long timeout_ms = timeout_s * 1000;

    Serial.println("Connecting to WiFi");
    unsigned long t0 = millis();
    while (!isConnectedToWifi() && ( (millis() - t0) < timeout_ms) ) {
        Serial.print(".");
    }

    if ((millis() - t0) > timeout_ms) {
        return false;
    }

    Serial.printf("Connected to WiFi %s\n", WiFi.localIP().toString().c_str());
    return true;
}
