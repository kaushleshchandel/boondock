/*
 Example : Records and saves a file to SD card.
 Use a FAT32 formatted SD card. create folder named "inbox".
 Newly recorded audio files will get stored on the SD card.

 TODO
 - Led functions
 - Button Press commands to enable & disable recording
 - Upload audio to server
 - Add all buttons and their functions.
 - Record on demand on button press.
 - SERVER SIDE - Accept audio files, save to database.


 - Fix the Audio sampling rate
 - Detect Mic & Speakers conencted

 - Add MQTT commands

*/
#include "commpn.h"

void setup()
{
  Serial.begin(115200);
  pinMode(GPIO_NUM_19, OUTPUT);

  digitalWrite(GPIO_NUM_19, HIGH); delay(100); digitalWrite(GPIO_NUM_19, LOW); delay(100);
  digitalWrite(GPIO_NUM_19, HIGH); delay(100); digitalWrite(GPIO_NUM_19, LOW); delay(100);
  digitalWrite(GPIO_NUM_19, HIGH); delay(100); digitalWrite(GPIO_NUM_19, LOW); delay(100);

 
  delay(500);

  //root = SD.open("/inbox/");
  //printDirectory(root, 0);

  Serial.print("Initializing SD card...");
  init_SD();
  init_mic();
  // capture_audio();
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

}