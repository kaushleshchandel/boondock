
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
String serverName = "10.0.2.68"; // OR REPLACE WITH YOUR DOMAIN NAME

#define PART_BOUNDARY "123456789000000000000987654321"

String serverPath = "/upload.php"; // The default serverPath should be upload.php

const int serverPort = 80;

WiFiClient client;

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

/******************************
connectWiFi()
Connects to WiFi
*******************************/
void connect_wifi()
{

    WiFi.mode(WIFI_STA);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("ESP32-CAM IP Address: ");
    Serial.println(WiFi.localIP());
}

/******************************
formatBytes()
Format Bytes for sending
*******************************/
String formatBytes(unsigned int bytes)
{
    if (bytes < 1024)
    {
        return String(bytes) + "B";
    }
    else if (bytes < (1024 * 1024))
    {
        return String(bytes / 1024.0) + "KB";
    }
    else if (bytes < (1024 * 1024 * 1024))
    {
        return String(bytes / 1024.0 / 1024.0) + "MB";
    }
}

/******************************
upload_file_to_server()
Upload file to the Server
*******************************/
void upload_file_to_server(String filename)
{
    // Intialize Wifi

    String getAll;
    String getBody;

    // read file from SD
    // define file
    myFile = SD.open(filename, FILE_READ);

    String fileName = myFile.name();
    String fileSize = formatBytes(myFile.size());

    Serial.println();
    Serial.println("file exists");

    String myMac = WiFi.macAddress();
    myMac.replace(":", "");

    if (myFile)
    {
        Serial.println("test file:ok");
        // print content length and host
        Serial.print("contentLength : ");
        Serial.println(fileSize);
        Serial.print("connecting to ");
        Serial.println("events.bluevan.io");

        // We now create a URI for the request
        Serial.println("Connected to server");
        Serial.print("Requesting URL: ");
        Serial.println("/boondock/upload.php");

        // Make a HTTP request and add HTTP headers
        // String boundary = "CustomizBoundarye----";
        // change with your content type
        String contentType = "audio/x-wav";
        String portString = String("80");
        String hostString = String("events.bluevan.io");

        String requestHead = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"audioFile\"; filename=\"" + myMac + "\"\r\nContent-Type: audio/x-wav\r\n\r\n";

        String tail = "\r\n--RandomNerdTutorials--\r\n";

        int contentLength = requestHead.length() + myFile.size() + tail.length();

        client.connect("events.bluevan.io", 80);

        client.println("POST /boondock/upload.php HTTP/1.1");
        client.println("Host: events.bluevan.io");
        client.println("Content-Length: " + String(contentLength, DEC));
        client.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
        client.println();
        client.print(requestHead);

        // send myFile:
        // this method is for upload data very fast
        // and only work in ESP software version after 2.3.0
        // if your version is lower than please update
        // esp software to last version or use bellow comment code
        client.write(myFile);
        // create file buffer
        const int bufSize = 2048;
        byte clientBuf[bufSize];
        int clientCount = 0;

        while (myFile.available())
        {
            clientBuf[clientCount] = myFile.read();
            clientCount++;
            if (clientCount > (bufSize - 1))
            {
                client.write((const uint8_t *)clientBuf, bufSize);
                clientCount = 0;
            }
        }

        if (clientCount > 0)
        {
            client.write((const uint8_t *)clientBuf, clientCount);
        }

        // send tail
        //      char charBuf3[tail.length() + 1];
        //      tail.toCharArray(charBuf3, tail.length() + 1);
        client.print(tail);

        // Serial.print(charBuf3);
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening test.WAV");
        Serial.println("Post Failure");
    }

    // Read all the lines of the reply from server and print them to Serial

    Serial.println("request sent");
    String responseHeaders = "";

    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;

    while ((startTimer + timoutTimer) > millis())
    {
        Serial.print(".");
        delay(100);
        while (client.available())
        {
            char c = client.read();
            if (c == '\n')
            {
                if (getAll.length() == 0)
                {
                    state = true;
                }
                getAll = "";
            }
            else if (c != '\r')
            {
                getAll += String(c);
            }
            if (state == true)
            {
                getBody += String(c);
            }
            startTimer = millis();
        }
        if (getBody.length() > 0)
        {
            break;
        }
    }
    Serial.println();
    client.stop();
    Serial.println(getBody);

    // close file
    myFile.close();
}

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
            /*
            uint8_t cardType = SD.cardType();
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
            */
            uint64_t cardSize = SD.cardSize() / (1024 * 1024);
            Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
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
    cfg.sample_rate = AUDIO_HAL_08K_SAMPLES;
    cfg.bits_per_sample = AUDIO_HAL_BIT_LENGTH_16BITS;
    //  cfg.bits_per_sample
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
    String filename = "";
    // WAVAudioInfo mywav = default_config;

    // mywav.sample_rate = AUDIO_HAL_48K_SAMPLES;
    // mywav.bits_per_sample = AUDIO_HAL_BIT_LENGTH_16BITS;
    bool file_created = false;
    int r = 0;
    while (r < 5)
    {
        filename = fileLocation + "a_" + String(random(99)) + "_" + String(random(999999)) + ".wav";
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

    upload_file_to_server(filename);
}

#endif