# THERMO32
This repository contains the design and source code for a smart temperature monitoring system. At it's core, the THERMO32 is a PCB which contains an [ESP32-C3](https://www.espressif.com/en/products/socs/esp32-c3) microcontroller and a [STS35](https://sensirion.com/products/catalog/STS35-DIS/) temperature sensor. The PCB comes with a USB-mini to power it, and contains a button that allows you to set it into setting-mode, where you can configure the settings through its web interface.

## Startup
When the device boots, it loads the settings and tries to connect to a WiFi with the stored WiFi SSID and password. If the device fails to connect to a WiFi within a given amount of time, it starts as an access point instead, and starts the server mode (see below) which you can connect to and configure its settings.

## Server mode
When the device is in server mode you can configure the settings.

The device can enter server mode in two ways:
1. If it fails to connect to the WiFi SSID in its configuration.
2. By holding down the button while you power on the device.

Once the device is in server mode, you can configure its settings through its webserver that you can access on port 80.
