/**
 * @file input.ino
 * @author Phil Schatzmann
 * @brief Input of audio data from the AudioKit microphones
 * @date 2021-12-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#define USE_SDFAT
#include "Arduino.h"
//#include "AudioKitHAL.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"

AudioKit kit;
File root;
File myFile;

const int BUFFER_SIZE = 1024;
uint8_t buffer[BUFFER_SIZE];

void printDirectory(File dir, int numTabs)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    for (uint8_t i = 0; i < numTabs; i++)
      Serial.print('\t');

    Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void printBuffer(int len)
{
  // by default we get int16_t values on 2 channels = 4 bytes per frame
  int16_t *value_ptr = (int16_t *)buffer;
  for (int j = 0; j < len / 4; j++)
  {
    Serial.print(*value_ptr++);
    Serial.print(", ");
    Serial.println(*value_ptr++);
  }
}

void init_mic()
{

  // open in read mode
  auto cfg = kit.defaultConfig(AudioInput);
  cfg.adc_input = AUDIO_HAL_ADC_INPUT_LINE2; // microphone
  cfg.sample_rate = AUDIO_HAL_08K_SAMPLES;
  kit.begin(cfg);
}

void capture_audio()
{
  String filename = "/inbox/audio" + String(random(999)) + ".wav";
  Serial.println("Saving to " + filename);
  myFile = SD.open(filename, FILE_WRITE);
  WAVEncoder encoder(myFile);
  encoder.begin();
  Serial.println("Creating WAV file...");

  for (int i; i < 1000; i++)
  {
    size_t len = kit.read(buffer, BUFFER_SIZE);
   // Serial.println("Read bytes " + String(len));
    if (myFile)
    {
      encoder.write(buffer, len);
     // Serial.print("+");
    }
    else
    {
      Serial.println("error opening " + filename);
      myFile.close();
      Serial.println("Closed File");
    }
  }

  myFile.close();
  Serial.println("Closed File");
}

void setup()
{
  Serial.begin(115200);

  Serial.print("Initializing SD card...");

  if (!SD.begin(kit.pinSpiCs(), AUDIOKIT_SD_SPI))
  {
    Serial.println("initialization failed. Things to check:");
    while (true)
      ;
  }

  Serial.println("initialization done.");

  root = SD.open("/inbox/");
  printDirectory(root, 0);

  Serial.print("Initializing SD card...");

  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  init_mic();
  capture_audio();
}

void loop()
{
  // size_t len = kit.read(buffer, BUFFER_SIZE);
  // printBuffer(len);
  // capture_audio();
}