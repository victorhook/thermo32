#include "settings_handler.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

const char* settings_path = "settings";
const settings_t default_settings = {

};
settings_t settings;

static bool waitForRestart = true;

// -- Helper methods -- /
void _storeSettings(const settings_t* settings) {
    File file = SPIFFS.open(settings_path, "w", true);
    size_t bytes_written = file.write((uint8_t*) settings, sizeof(settings_t));
    file.close();

    if (bytes_written != sizeof(settings_t)) {
        // TOOD?
    }
}

void _loadSettings(settings_t* settings) {
    File file = SPIFFS.open(settings_path, "r");
    size_t bytes_read = file.readBytes((char*) settings, sizeof(settings_t));
    file.close();

    if (bytes_read != sizeof(settings_t)) {
        // TOOD?
    }
}

static void setSettingParamString(char* str, AsyncWebServerRequest *request, const char* name);
static void setSettingParamU16(uint16_t* u16, AsyncWebServerRequest *request, const char* name);
// -- Http request handlers -- //
static void onNotFound(AsyncWebServerRequest *request);
static void onIndex(AsyncWebServerRequest *request);
static void onSettings(AsyncWebServerRequest *request);
static void onRestart(AsyncWebServerRequest *request);


SettingsHandler::SettingsHandler() {

}

void SettingsHandler::startServer() {
    //_settings = settings;
    AsyncWebServer server(SETTINGS_SERVER_PORT);
    server.on("/", HTTP_GET, onIndex);
    server.on("/settings", HTTP_POST, onSettings);
    server.on("/restart", HTTP_POST, onRestart);
    server.onNotFound(onNotFound);
    server.begin();

    while (waitForRestart);
}

const settings_t* SettingsHandler::loadSettings() {
    SPIFFS.begin();
    if (!SPIFFS.exists(settings_path)) {
        _storeSettings(&default_settings);
    }

    _loadSettings(&settings);
    return &settings;
}

// -- Private -- //

// Helpers
static void setSettingParamString(char* str, AsyncWebServerRequest *request, const char* name) {
    if (request->hasParam(name)) {
        AsyncWebParameter* param = request->getParam(name);
        memcpy(str, param->value().c_str(), param->size());
    }
}

static void setSettingParamU16(uint16_t* u16, AsyncWebServerRequest *request, const char* name) {
    if (request->hasParam(name)) {
        AsyncWebParameter* param = request->getParam(name);
        *u16 = param->value().toInt();
    }
}

static String processor(const String& var)
{
    if (var == "ssid") {
        return String(settings.wifi_ssid);
    } else if (var == "password") {
        return String(settings.wifi_pass);
    } else if (var == "wifi_timeout") {
        return String(settings.wifi_connect_timeout);
    } else if (var == "device_id") {
        return String(settings.device_id);
    } else if (var == "mqtt_broker_ip") {
        return String(settings.mqtt_broker_ip);
    } else if (var == "mqtt_broker_port") {
        return String(settings.mqtt_broker_port);
    } else if (var == "mqtt_topic") {
        return String(settings.mqtt_topic);
    } else if (var == "pub_freq") {
        return String(settings.min_between_pub);
    }

  return String();
}

// Http request handlers
static void onNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
static void onIndex(AsyncWebServerRequest *request) {
    _loadSettings(&settings);
    request->send(SPIFFS, "/index.html", "text/plain", false, processor);
}
static void onSettings(AsyncWebServerRequest *request) {
    settings_t settings;
    setSettingParamString(settings.wifi_ssid, request, "ssid");
    setSettingParamString(settings.wifi_pass, request, "password");
    setSettingParamU16(&settings.wifi_connect_timeout, request, "wifi_timeout");
    setSettingParamString(settings.device_id, request, "device_id");
    setSettingParamString(settings.mqtt_broker_ip, request, "mqtt_broker_ip");
    setSettingParamU16(&settings.mqtt_broker_port, request, "mqtt_broker_port");
    setSettingParamString(settings.mqtt_topic, request, "mqtt_topic");
    setSettingParamU16(&settings.min_between_pub, request, "min_between_pub");
    _storeSettings(&settings);
    _loadSettings(&settings);
    request->send(200, "text/plain", "Settings set OK");
}
static void onRestart(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Restarting...");
    waitForRestart = false;
}