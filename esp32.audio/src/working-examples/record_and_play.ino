#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/MemoryManager.h"

uint16_t sample_rate = 16000;
uint8_t channels = 1;  // The stream will have 1 channel
AudioKitStream kit;
MemoryManager memory(500); // Activate SPI RAM for objects > 500 bytes
DynamicMemoryStream recording(true); // Audio stored on heap looping endlessly 
StreamCopy copier; // copies data


void record_start(bool pinStatus, int pin, void* ref){
  Serial.println("Recording...");
  recording.begin(); // clear data
  copier.begin(recording, kit); // from mic to memory 
}

void record_end(bool pinStatus, int pin, void* ref){
  Serial.println("Playing...");
  copier.begin(kit, recording);  // from memory to kit
}

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // setup input and output
  auto cfg = kit.defaultConfig(RXTX_MODE);
  cfg.sd_active = false; // we don't need the SD card
  cfg.sample_rate = sample_rate; 
  cfg.channels = channels;
  cfg.input_device = AUDIO_HAL_ADC_INPUT_LINE2;
  kit.begin(cfg);
  kit.setVolume(1.0);

  // record when key 1 is pressed
  kit.audioActions().add(PIN_KEY1, record_start, record_end);
  Serial.println("Press Key 1 to record");

}

void loop(){
  // record or play recording
  copier.copy();  

  // Process keys
  kit.processActions();
}