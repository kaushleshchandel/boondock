#ifndef __BOONDOCK_CONFIG__
#define __BOONDOCK_CONFIG__

#include <Arduino.h>

String serverName = "servername.com"; // Or replace with your own domain
String serverPath = "/upload.php";       // The default serverPath should be upload.php
String fileLocation = "/inbox/";         // Location on SD card where files are saved

const char *mqtt_server = "mqtt_server_ip_address";

const char *mqttUser = "mqtt_user";
const char *mqttPassword = "mqtt_password";
const char *mqttClientID = "ESP32BoonDock";

String ssid = "WIFI_SSID";             // Change with your Wifi Router
String password = "WIFI_PASSWORD";   // Your wifi Router Password

#endif
