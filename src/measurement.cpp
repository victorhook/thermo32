#include "measurement.h"
#include <Arduino.h>
#include "PubSubClient.h"
#include <WiFiClient.h>
#include <Wire.h>
#include "ClosedCube_STS35.h"


static ClosedCube::Sensor::STS35 temp_sensor;


int initSensors() {
    Wire.begin();
    temp_sensor.address(0x4A);
    return 0;
}

int takeMeasurement(measurement_t* measurement) {

    float temperature = temp_sensor.readTemperature();
    measurement->temperature = temperature;
    measurement->timestamp = millis();
    return 0;
}

int publishMeasurement(const measurement_t* measurement, const settings_t* settings) {
    WiFiClient client;
    IPAddress broker_ip;
    if (!broker_ip.fromString(settings->mqtt_broker_ip)) {
        // Failed to parse IP, TOOD.
    }

    PubSubClient mqtt(broker_ip, settings->mqtt_broker_port, client);

    String data = String("{") +
                  String("device_id: ") + String(settings->device_id) + String(",") +
                  String("temperature: ") + String(measurement->temperature) + "," +
                  String("timestamp: ") + String(measurement->timestamp) +
                  String("}");
    mqtt.publish(settings->mqtt_topic, data.c_str());

    return 0;
}
