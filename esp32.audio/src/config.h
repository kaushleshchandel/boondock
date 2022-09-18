#ifndef __BOONDOCK_CONFIG__
#define __BOONDOCK_CONFIG__

#include <Arduino.h>

String serverName = "XXXXXX"; // Or replace with your own domain
String serverPath = "/upload.php";       // The default serverPath should be upload.php
String fileLocation = "/inbox/";         // Location on SD card where files are saved

const char *mqtt_server = "XXXX";

const char *mqttUser = "XXXX";
const char *mqttPassword = "XXXX";
const char *mqttClientID = "XXXX";

String ssid = "XXXX";             // Change with your Wifi Router
String password = "XXXXX";   // Your wifi Router Password

#endif
