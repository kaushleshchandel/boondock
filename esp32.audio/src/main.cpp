/*
 Example : Records and saves a file to SD card.
 Use a FAT32 formatted SD card. create folder named "inbox".
 Newly recorded audio files will get stored on the SD card.

 TODO

 - Upload audio to server
 - Playback the Recorded messages
 - ?? Can it Playback while checking for Audio in?
 - Push to Record ( Sends audio to Server )
 
 - Say the Status on button press
 - Mirror the Audio in to Speaker out
 - Button Press to switch the Microphone Input
 
 
 - Cycle oldest Audio file
 - Button Press to Clean up storage
 - Add all buttons and their functions.
 - Record on demand on button press.
 - SERVER SIDE - Accept audio files, save to database.


 - Fix the Audio sampling rate
 - Detect Mic & Speakers conencted

 - Add MQTT commands
 - Button Press commands to enable & disable recording

*/
#include "commpn.h"

void setup()
{
  Serial.begin(115200);
 
  delay(500);

  //root = SD.open("/inbox/");
  //printDirectory(root, 0);

  Serial.print("Initializing SD card...");
  init_SD();
  init_mic();
 
}

void loop()
{
  if(current_state == STATE_SD_WRITE_ERROR)
  {
    Serial.print("Disk Write Error");
    esp_restart();
  }

  current_state = STATE_LISTENING;

  size_t len = kit.read(buffer, BUFFER_SIZE);
  if (MeasureAnalog())
  {
    Serial.println("Audio detected");
    current_state = STATE_RECORDING;
    capture_audio();
  }
  //  delay(1);
}