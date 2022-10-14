
#include "Arduino.h"

#define SW_VERSION "1.0"
#define HW_VERSION "ait2.2"
#define FILE_LOCATION "https://app.boondock.live/"

/**************** CHANGE THESE VARIABLES AS NEEDED ****************************/
#define INPUT_LINE AUDIO_HAL_ADC_INPUT_LINE2        // Uses Mic & Line input
#define DEFAULT_AUDIO_ON_SPEAKER false              // Play Audio on the speaker?
#define DEFAULT_SILENSE_BEFORE_RECORDING_STOPS 1000 // How long is it silent before recording stops; in milliseconds
#define DEFAULT_MIN_RECORDING_DURATION 3000         // Minimum recording file duration; in Milliseconds
#define DEFAULT_MAX_RECORDING_DURATION 10000        // Maximum recording file duration; in Milliseconds
#define DEFAULT_MIN_SOUND 20                        // How much sound before recording starts Range 0 to 100;
#define DEFAULT_SPEAKER_VOLUME 50                   // How much sound before recording starts Range 0 to 100;

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
#define SAMPLING_BUFFER_SIZE 32                     // Buffer samples taken to listen for sound activity
#define RECORDING_BUFFER_SIZE 2048                  //Buffer samples used to save the audio file\
#define SAMPLING_RATE AUDIO_HAL_08K_SAMPLES         // Sampling rate
#define BITS_PER_SAMPLE AUDIO_HAL_BIT_LENGTH_16BITS // Bitrate
#define DEFAULT_SAMPLE_RATE 16000
#define DEFAULT_BITS_PER_SAMPLE 16
#define DEFAULT_CHANNELS 1

bool sd_card_available = false;
int _FILE_SIZE = 0;

/**************** THESE ARE IMPORTANT BUT MESS WITH THESE WHEN U R SURE *********/

#include "common.h" //Common.h file holds all the reference and functions

void setup()
{
  Serial.begin(115200);

  connect_wifi(ssid, password); // Connect to wifi
  get_config();
  mqtt_reconnect();                                         // Connect to MQTT
  mqtt_send_summary();                                      // Send MQTT Status & Config data
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // Configure time

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  i2s.begin(config);

  //   audio_player("https://www2.cs.uic.edu/~i101/SoundFiles/CantinaBand60.wav");
}

void loop()
{

  if (WiFi.status() != WL_CONNECTED)
    connect_wifi(ssid, password);
  mqttClient.loop();
  process_led();
  process_buttons();
  // Serial.print(".");
}
