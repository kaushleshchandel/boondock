
#define STATE_INIT 0
#define STATE_LISTENING 1
#define STATE_RECORDING 2
#define STATE_SENDING 3
#define STATE_SD_WRITE_ERROR 4

int current_state = 0;

#define USE_SDFAT
#include "Arduino.h"
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"

String fileLocation = "/inbox/";

AudioKit kit;
File root;
File myFile;

uint16_t sample_rate = 16000;
uint8_t channels = 1; // The stream will have 1 channel

const int BUFFER_SIZE = 1024;
uint16_t buffer[BUFFER_SIZE];

// List dir in SD card
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

// Create a dir in SD card
void createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

// delete a dir in SD card
void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

// Read a file in SD card
void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
}

// Write a file in SD card
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    // fwrite(fb->buf, 1, fb->len, file);
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
}

// Append to the end of file in SD card
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
}

// Rename a file in SD card
void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}

// Delete a file in SD card
void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

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

void init_filesystem()
{
    // Cleaning up first
    if (SD.exists("/inbox") == true)
    {
        Serial.println("Removing inbox Directory");
        removeDir(SD, "/inbox");
    }

    if (SD.exists("/logs") == true)
    {
        Serial.println("Removing logs Directory");
        removeDir(SD, "/logs");
    }
    if (SD.exists("/system") == true)
    {
        Serial.println("Removing system Directory");
        removeDir(SD, "/system");
    }

    if (SD.exists("/inbox") == false)
    {
        Serial.println("Creating Inbox Directory");
        createDir(SD, "/inbox");
    }

    if (SD.exists("/system") == false)
    {
        Serial.println("Create System Directory");
        createDir(SD, "/system");
    }
    if (SD.exists("/logs") == false)
    {
        Serial.println("Create Logs Directory");
        createDir(SD, "/logs");
    }
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
            init_filesystem();
            res = true;
            break;
        }
        r++;
    }
    return res;
}

// measure basic properties of the input signal
// determine if analog or digital, determine range and average.

int MIN_SOUND = -10;

bool MeasureAnalog()
{
    uint16_t signalAvg = 0, signalMax = 0, signalMin = 1024, t0 = millis();
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        uint16_t k = buffer[i];
        signalMin = min(signalMin, k);
        signalMax = max(signalMax, k);
        signalAvg += k;
    }
    signalAvg /= BUFFER_SIZE;
    if ((signalMax - 65530) < MIN_SOUND)
        return true;
    else
        return false;

}

long silentSince = 0;
#define SILENSE_THRESHOLD 1000

// Check for Silence
bool is_silent()
{
    if (MeasureAnalog())
    {
        silentSince = 0;
        Serial.print("+");
        return false;
    }
    else
    {
        silentSince++;
        if (silentSince > SILENSE_THRESHOLD)
        {
            Serial.print("-");
            silentSince = 0;
            return true;
        }
        else
        {
            Serial.print(".");
            return false;
        }
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

    for (int i = 0; i < 100000; i++)
    {
        size_t len = kit.read(buffer, BUFFER_SIZE);

        if (i > 100)
        {
            if(is_silent() == true)
            {
                Serial.println("No more sound");
                myFile.close();
                break;
            }
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
            Serial.println("Closed File");
            current_state = STATE_SD_WRITE_ERROR;
            break;
        }
    }
    if (myFile)
    {
        myFile.close();
        Serial.println("Closed File");
    }
}
