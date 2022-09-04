/*
 Example : Records and saves a file to SD card.
 Use a FAT32 formatted SD card. create folder named "inbox".
 Newly recorded audio files will get stored on the SD card.
*/


#define USE_SDFAT
#include "Arduino.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"

AudioKit kit;
File root;
File myFile;

uint16_t sample_rate = 16000;
uint8_t channels = 1;  // The stream will have 1 channel

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

/*
void init_mic()
Initialize the Microphone
Sampling rate must match the Wav Encoder
*/
void init_mic()
{
  auto cfg = kit.defaultConfig(AudioInput);
  cfg.adc_input = AUDIO_HAL_ADC_INPUT_LINE2; // microphone
  cfg.sample_rate = AUDIO_HAL_48K_SAMPLES;
 // cfg.bitsPerSample = AUDIO_HAL_B
  kit.begin(cfg);
}


/*
void capture_audio()
Captures an Audio sample
Sampling rate must match the Wav Encoder
*/
void capture_audio()
{

  WAVAudioInfo mywav = default_config;
  
 // mywav.sample_rate = AUDIO_HAL_48K_SAMPLES;
 // mywav.bits_per_sample = AUDIO_HAL_BIT_LENGTH_16BITS;

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