#ifndef __BOONDOCK_CONFIG__
#define __BOONDOCK_CONFIG__

#include <Arduino.h>

String serverName = "0.0.0.0"; //  replace with your own domain
String serverPath = "/upload.php";       // The default serverPath should be upload.php
String fileLocation = "/inbox/";         // Location on SD card where files are saved

const char *mqtt_server = "0.0.0.0"; //  replace with your own domain

const char *mqttUser = "mqttuser";
const char *mqttPassword = "mqttpassword";
const char *mqttClientID = "ESP32BoonDock";

String ssid = "WIFI-SSID";             // Change with your Wifi Router
String password = "WIFI-PASSWORD";   // Your wifi Router Password

#endif
