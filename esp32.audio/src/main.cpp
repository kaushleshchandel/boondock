/*
Boondock echo 1.0

TODO :
* Play mp3 audio from URL
* Play audio from file stored on SD Card
* Setting up Wifi using WiFi Manager Library
* OTA functionality to automatically download latest firmware

 */


#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"


URLStream url("***","*********");
 
AudioKitStream i2s; // final output of decoded stream
EncodedAudioStream dec(&i2s, new MP3DecoderHelix()); // Decoding stream
StreamCopy copier(dec, url); // copy url to decoder


void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  i2s.begin(config);

  // setup I2S based on sampling rate provided by decoder
  dec.begin();

  // mp3 radio
  url.begin("http://events.bluevan.io/boondock/audio/welcome.mp3","audio/mp3");

}

void loop(){
  copier.copy();
}
