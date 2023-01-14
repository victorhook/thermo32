#include <Arduino.h>
#include "wifi_handler.h"
#include "settings_handler.h"
#include "led_handler.h"
#include "measurement.h"


typedef enum {
  MODE_NORMAL,
  MODE_SET_CONFIGURATION,
} thermo32_mode_t;

#define CONNECT_TO_WIFI_TIMEOUT_S 30

thermo32_mode_t mode = MODE_NORMAL;
WiFiHandler wifi;
SettingsHandler settings_handler;


void setup() {

  switch (esp_sleep_get_wakeup_cause()) {
    case ESP_SLEEP_WAKEUP_TIMER:
      break;
    default:
      break;
  }

  led_init(&led_blue);
  led_init(&led_red);
  Serial.begin(115200);

  Serial.println("Initializing sensors...");
  initSensors();

  // Once we've booted, we'll try to find a WiFi to connect to.
  Serial.println("Initializing WiFi...");
  led_set(&led_blue, LED_MODE_BLINK_0_5_HZ);
  if (!wifi.connectToWiFi(CONNECT_TO_WIFI_TIMEOUT_S)) {
    // Failed to connect to WiFi.
    led_set(&led_blue, LED_MODE_BLINK_2_HZ);
    wifi.startAsAccessPoint();
    mode = MODE_SET_CONFIGURATION;
  }

  if (mode == MODE_SET_CONFIGURATION) {
    // We're not going to do any measurements, but instead start a server
    // so the user can configuration some settings.
    led_set(&led_blue, LED_MODE_ON);
    settings_handler.startServer();

    // Once the settings-handler server is finished, we'll restart.
    esp_restart();
  }

}

void loop() {
  const settings_t* settings = settings_handler.loadSettings();
  measurement_t measurement;
  takeMeasurement(&measurement);
  publishMeasurement(&measurement, settings);

  // Go to sleep for X minutes.
  esp_sleep_enable_timer_wakeup((settings->min_between_pub * 60) * 1e6);
  esp_deep_sleep_start();
}
