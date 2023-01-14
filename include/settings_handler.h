#ifndef SETTINGS_HANDLER_H
#define SETTINGS_HANDLER_H

#include "stdint.h"


#define SETTINGS_SERVER_PORT 80


typedef struct {
    char     wifi_ssid[50];
    char     wifi_pass[50];
    uint16_t wifi_connect_timeout;
    char     device_id[50];
    char     mqtt_broker_ip[50];
    uint16_t mqtt_broker_port;
    char     mqtt_topic[50];
    uint16_t min_between_pub;
} __attribute__((packed)) settings_t;


class SettingsHandler {
    public:
        SettingsHandler();
        void startServer();
        const settings_t* loadSettings();
        void storeSettings();

    private:
        void createDefaultSettings();
};


#endif /* SETTINGS_HANDLER_H */
