/**
   @file input.ino
   @author Phil Schatzmann
   @brief Input of audio data from the AudioKit microphones
   @date 2021-12-10

   @copyright Copyright (c) 2021

*/
#include "AudioKitHAL.h"

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "SD.h"

String fname = "/" + String(random(999)) + ".wav";

AudioKit kit;
const int BUFFER_SIZE = 512;
uint8_t buffer[BUFFER_SIZE];

void printBuffer(int len) {
  // by default we get int16_t values on 2 channels = 4 bytes per frame
  int16_t *value_ptr = (int16_t*) buffer;
  for (int j = 0; j < len / 4; j++) {
    Serial.print(*value_ptr++);
    Serial.print(", ");
    Serial.println(*value_ptr++);
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {

      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void setup() {
  Serial.begin(115200);
  auto cfg = kit.defaultConfig(AudioInput);
  cfg.adc_input = AUDIO_HAL_ADC_INPUT_LINE1; // microphone
  cfg.sample_rate = AUDIO_HAL_16K_SAMPLES;
  kit.begin(cfg);

  Serial.println("Begin");
  SD.begin(kit.pinSpiCs(), AUDIOKIT_SD_SPI);
  Serial.println("SD Begin");
  File dataFile = SD.open(fname, FILE_WRITE);
  Serial.println("SD Open + " + fname);
  WAVEncoder encoder(dataFile);
  Serial.println("Encoder");

  // setup encoder
  encoder.begin();
  Serial.println("Creating WAV file...");
   static uint8_t buffer[512];

  Serial.println("Writing to file...");
  for (int i; i < 100; i++) {
    Serial.print("In the loop + ");
    // int len = music.readBytes(buffer, 512);
    size_t len = kit.read(buffer, BUFFER_SIZE);
    Serial.print("Kit read " + String(i));
    encoder.write(buffer, len);
    Serial.println(" Buffer write");
  }

  dataFile.close();
   Serial.println("File Close");
}
int counter = 0;


void loop() {
 
 
}
