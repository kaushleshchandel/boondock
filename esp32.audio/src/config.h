#ifndef __BOONDOCK_CONFIG__
#define __BOONDOCK_CONFIG__

#include <Arduino.h>

String serverName = "app.boondock.live"; // Or replace with your own domain
String serverPath = "/upload.php";       // The default serverPath should be upload.php
String fileLocation = "/inbox/";         // Location on SD card where files are saved

const char *mqtt_server = "3.137.29.71";

const char *mqttUser = "blueman";
const char *mqttPassword = "TYgu6fPhoofJ";
const char *mqttClientID = "ESP32BoonDock";

String ssid = "AAA";             // Change with your Wifi Router
String password = "608980608980";   // Your wifi Router Password

#endif
