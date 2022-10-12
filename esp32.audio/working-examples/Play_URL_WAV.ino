/**
 * @file streams-url_mp3-audiokit.ino
 * @author Phil Schatzmann
 * @brief decode MP3 stream from url and output it on I2S on audiokit
 * @version 0.1
 * @date 2021-96-25
 *
 * @copyright Copyright (c) 2021
 */

// install https://github.com/pschatzmann/arduino-libhelix.git

#define DEFAULT_SAMPLE_RATE 16000
#define DEFAULT_BITS_PER_SAMPLE 16
#define DEFAULT_CHANNELS 1

int _FILE_SIZE = 0;

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"

AudioKitStream i2s;                             // final output of decoded stream
EncodedAudioStream dec(&i2s, new WAVDecoder()); // Decoding stream

void setup()
{
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Error);

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  i2s.begin(config);
}

URLStream url("AAA", "608980608980"); // or replace with ICYStream to get metadata

void play_link(String link)
{

  StreamCopy copier(dec, url);          // copy url to decoder
  // setup I2S based on sampling rate provided by decoder
  dec.begin();
  url.begin(link.c_str(), "audio/wav");

  if (_FILE_SIZE > 0)
    Serial.println("File size is " + String(_FILE_SIZE));

  int packet_count = (_FILE_SIZE / 1024) + 5;

  while (packet_count > 0)
  {
    copier.copy();
    packet_count--;
  }

  url.end();
  copier.end();
}

void loop()
{

  play_link("https://app.boondock.live/uploads/C4DEE22006F4/2022_09_28_10_49_23.wav");

  delay(1000);

  play_link("https://www2.cs.uic.edu/~i101/SoundFiles/CantinaBand60.wav");

  delay(1000);

  play_link("https://www2.cs.uic.edu/~i101/SoundFiles/CantinaBand3.wav");
    delay(1000);

  play_link("https://www2.cs.uic.edu/~i101/SoundFiles/gettysburg.wav");
    delay(1000);

  play_link("https://www2.cs.uic.edu/~i101/SoundFiles/StarWars60.wav");
    delay(1000);

  play_link("https://www2.cs.uic.edu/~i101/SoundFiles/PinkPanther30.wav");

}
