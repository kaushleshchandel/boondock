
#ifndef __BOONDOCK_COMMON__
#define __BOONDOCK_COMMON__

#define STATE_INIT 0
#define STATE_LISTENING 1
#define STATE_RECORDING 2
#define STATE_SENDING 3
#define STATE_SD_WRITE_ERROR 4

#define MAX_FILE_DURATION 60
#define SILENSE_THRESHOLD 3000 // in MilliSeconds
#define MAX_AUDIO_BUFFER 10000
#define MIN_SOUND 50 // Range 0 to 100

#define SAMPLING_BUFFER_SIZE 64
#define RECORDING_BUFFER_SIZE 1024

int current_state = 0;

#define USE_SDFAT
#include "Arduino.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "data.h"
#include <WiFi.h>
#include "time.h"

const char *ssid = "AAA";
const char *password = "608980608980";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

long silentSince = 0;

String fileLocation = "/inbox/";

AudioKit kit;
File root;
File myFile;

uint16_t sample_rate = 16000;
uint8_t channels = 1; // The stream will have 1 channel

uint16_t buffer[RECORDING_BUFFER_SIZE];

// Time function
void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

bool init_SD()
{
    bool res = false;
    Serial.print("Initializing SD card...");

    int r = 0;
    while (r < 5)
    {
        if (!SD.begin(kit.pinSpiCs(), AUDIOKIT_SD_SPI))
        {
            Serial.println("initialization failed. Try again... " + String(r));
        }
        else
        {
            uint8_t cardType = SD.cardType();
            /*
                        if (cardType == CARD_NONE)
                            Serial.println("No SD_MMC card attached");

                        Serial.print("SD_MMC Card Type: ");
                        if (cardType == CARD_MMC)
                            Serial.println("MMC");
                        else if (cardType == CARD_SD)
                            Serial.println("SDSC");
                        else if (cardType == CARD_SDHC)
                            Serial.println("SDHC");
                        else
                            Serial.println("UNKNOWN");

                        uint64_t cardSize = SD.cardSize() / (1024 * 1024);
                        Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
            */
            Serial.println("initialization done.");
            init_filesystem(SD);
            res = true;
            break;
        }
        r++;
    }
    return res;
}

// measure basic properties of the input signal
// Return if there is Sound
bool measure_sound(bool sample = false)
{
    int16_t signalMax = 0, signalMin = 1024, signalTotal = 0;
    int soundMeasure = 0;
    int buffer_Size = 0;

    if (sample)
        buffer_Size = SAMPLING_BUFFER_SIZE;
    else
        buffer_Size = RECORDING_BUFFER_SIZE;

    for (int i = 0; i < buffer_Size; i++)
    {
        int16_t k = buffer[i];
        signalMin = min(signalMin, k);
        signalMax = max(signalMax, k);
    }

    signalTotal = abs(signalMin) + abs(signalMax);

    soundMeasure = map(signalTotal, 0, 20000, 0, 100);

    //  Serial.print("Sound:"); Serial.print(soundMeasure); Serial.println(",");

    return (soundMeasure > MIN_SOUND); // Returns true when sound is greater than Thrashold
}

// Check for Silence
bool is_silent()
{
    Serial.print(">");
    long currentMillis = millis();

    if (measure_sound(false))
    {
        silentSince = millis();
        return false;
    }
    else if ((currentMillis - silentSince) > SILENSE_THRESHOLD)
        return true;
    else
        return false;
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
    silentSince = millis();
    unsigned long StartTime = millis(); // Startime for the File

    WAVAudioInfo mywav = default_config;

    // mywav.sample_rate = AUDIO_HAL_48K_SAMPLES;
    // mywav.bits_per_sample = AUDIO_HAL_BIT_LENGTH_16BITS;
    bool file_created = false;
    int r = 0;
    while (r < 5)
    {
        String filename = fileLocation + "a_" + String(random(99)) + "_" + String(random(999999)) + ".wav";
        Serial.println("Opening file for read write : " + filename);
        myFile = SD.open(filename, FILE_WRITE);
        if (myFile)
        {
            file_created = true;
            break;
        }
        else
            r++;
    }

    WAVEncoder encoder(myFile);
    encoder.begin();

    if (file_created)
    {
        Serial.println("Saving to WAV file...");
    }

    unsigned long CurrentTime = millis();
    unsigned long ElapsedTimeinSeconds = CurrentTime - StartTime;

    for (int i = 0; i < MAX_AUDIO_BUFFER; i++)
    {
        ElapsedTimeinSeconds = (CurrentTime - StartTime) / 1000;

        if (ElapsedTimeinSeconds > MAX_FILE_DURATION)
        {
            Serial.println("Closing file by Duration");
            myFile.close();
            break;
        }
        size_t len = kit.read(buffer, RECORDING_BUFFER_SIZE);

        if (is_silent() == true)
        {
            Serial.println("Closing file due to silence");
            myFile.close();
            break;
        }

        // Serial.println("Read bytes " + String(len));
        if (myFile)
        {
            encoder.write(buffer, len);
            // Serial.print("+");
        }
        else
        {
            //  Serial.println("error opening " + filename);
            myFile.close();
            Serial.println("Closing file due to error");
            current_state = STATE_SD_WRITE_ERROR;
            break;
        }
    }

    if (myFile)
    {
        myFile.close();
        Serial.println("Closing file by max buffers");
    }
    CurrentTime = millis(); // Get Current Time
}

#endif