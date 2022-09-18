/*
Reads Microphone or Line in and upoads it to the server

*/

#include "Arduino.h"

/**************** CHANGE THESE VARIABLES AS NEEDED ****************************/
#define INPUT_LINE AUDIO_HAL_ADC_INPUT_LINE2 // Uses Mic & Line input
#define AUDIO_ON_SPEAKER true               // Play Audio on the speaker?
#define SILENSE_BEFORE_RECORDING_STOPS 3000  // How long is it silent before recording stops; in milliseconds
#define MAX_RECORDING_DURATION 60            // Maximum recording file duration; in seconds
#define MIN_SOUND 50                         // How much sound before recording starts Range 0 to 100;

#define DEBUG_NONE 0
#define DEBUG_AUDIO 1
#define DEBUG_WIFI 2
#define DEBUG_ALL 3

#define DEBUG DEBUG_ALL // Set to true when you want to see all the useless messages scrolling on serial port
// Debug mode = 0
// Debug mode = 1 Switches to Ossicloscope mode for Arduino plotter
// Debug mode = 2 Shows
// Debug mode = 3 Shows all messages on the sSerial monitor
/**************** CHANGE THESE VARIABLES AS NEEDED ****************************/

/**************** THESE ARE IMPORTANT BUT MESS WITH THESE WHEN U R SURE *********/
#define MAX_AUDIO_BUFFER 10000                      // Maximum bufferes processed before recording is stopped
#define SAMPLING_BUFFER_SIZE 32                    // Buffer samples taken to listen for sound activity
#define RECORDING_BUFFER_SIZE 2048                  //Buffer samples used to save the audio file\
#define SAMPLING_RATE AUDIO_HAL_08K_SAMPLES         // Sampling rate
#define BITS_PER_SAMPLE AUDIO_HAL_BIT_LENGTH_16BITS // Bitrate
#define DEFAULT_SAMPLE_RATE 16000
#define DEFAULT_BITS_PER_SAMPLE 16
#define DEFAULT_CHANNELS 1

/**************** THESE ARE IMPORTANT BUT MESS WITH THESE WHEN U R SURE *********/

#include "common.h" //Common.h file holds all the reference and functions

/******************************
setup()
Setup the hardware
*******************************/
void setup()
{
  Serial.begin(115200);

  Serial.println("Connecting to wifi...");
  // wm_setup(false);
  connect_wifi(ssid, password);
  mqtt_reconnect();

  // init and get the time
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Initializing SD card...");
  init_SD();
  Serial.println("Initializing Mic...");
  init_mic();
  mqtt_send_error("Hello");
}

/******************************
setup()
Setup the hardware
*******************************/
void loop()
{

  current_state = STATE_LISTENING;

  size_t len = kit.read(buffer, SAMPLING_BUFFER_SIZE);
   
  if (check_sound(len) &&  DEBUG != DEBUG_AUDIO)
  {
    // Serial.println("Audio detected");
    if (DEBUG == DEBUG_NONE)
      Serial.print("+");
    current_state = STATE_RECORDING;
    // printLocalTime();
    String fname = capture_audio(len);
    if (fname == "Error")
    {
      Serial.print(" Capture Error ");
    }
    else
    {
      current_state = STATE_SENDING;
      if (DEBUG == DEBUG_NONE)
        Serial.print("+");
      if (upload_file_to_server(fname))
        if (DEBUG == DEBUG_NONE)
          Serial.println("OK");
        else
          Serial.println(" File Send Error ");
    }
  }

  mqttClient.loop();
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
}
