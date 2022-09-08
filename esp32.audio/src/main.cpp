/*
Reads Microphone or Line in and upoads it to the server

*/

#include "common.h"

String ssid = "AAA";              // Change with your Wifi Router
String password = "608980608980"; // Your wifi Router Password

/******************************
setup()
Setup the hardware
*******************************/
void setup()
{
  Serial.begin(115200);

  Serial.println("Connecting to wifi...");
  connect_wifi(ssid, password);

  // init and get the time
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Initializing SD card...");
  init_SD();
  Serial.println("Initializing Mic...");
  init_mic();
}

/******************************
setup()
Setup the hardware
*******************************/
void loop()
{
  if (current_state == STATE_SD_WRITE_ERROR)
  {
    Serial.print("Disk Write Error");
    esp_restart();
  }
  else if (current_state == STATE_SD_WRITE_ERROR)
  {
    Serial.print("Disk Read Error");
    esp_restart();
  }
  else if (current_state == STATE_SD_WRITE_ERROR)
  {
    Serial.print("Audio file upload Error");
    esp_restart();
  }

  current_state = STATE_LISTENING;

  size_t len = kit.read(buffer, SAMPLING_BUFFER_SIZE);
  if (measure_sound(true))
  {
    // Serial.println("Audio detected");
    Serial.print("+");
    current_state = STATE_RECORDING;
    // printLocalTime();
    String fname = capture_audio();
    if (fname == "Error")
    {
      Serial.print(" Capture Error ");
    }
    else
    {
      current_state = STATE_SENDING;
      Serial.print("+");
      if (upload_file_to_server(fname))
        Serial.println("OK");
      else
        Serial.println(" File Send Error ");
    }
  }
}