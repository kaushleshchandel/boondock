/*
Functions for saving data to SD card
*/

#ifndef __BOONDOCK_DATA__
#define __BOONDOCK_DATA__

#include "Arduino.h"

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
        if (DEBUG == DEBUG_ALL)

            Serial.println("Dir created");
    }
    else
    {
        if (DEBUG == DEBUG_ALL)

            Serial.println("mkdir failed");
    }
}

// delete a dir in SD card
void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        if (DEBUG == DEBUG_ALL)
            Serial.println("Dir removed");
    }
    else
    {
        if (DEBUG == DEBUG_ALL)
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

// Cleans a Directory recursively
void cleanDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    if (DEBUG == DEBUG_ALL)
        Serial.printf("Cleaning directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        if (DEBUG == DEBUG_ALL)
            Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        if (DEBUG == DEBUG_ALL)
            Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            if (levels)
            {
                cleanDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            String fileWithPath = "/inbox/" + String(file.name());

            if (fs.remove(fileWithPath))
            {
                if (DEBUG == DEBUG_ALL)
                    Serial.println(fileWithPath + "File deleted");
            }
            else
            {
                if (DEBUG == DEBUG_ALL)
                    Serial.println(fileWithPath + "Delete failed");
            }
        }
        file = root.openNextFile();
    }
}

// Initialize the File System for read write
void init_filesystem(fs::FS &fs)
{
    // Cleaning up first

    cleanDir(fs, "/inbox", true);

    if (SD.exists("/inbox") == false)
        createDir(fs, "/inbox");

    if (fs.exists("/system") == false)
        createDir(fs, "/system");

    if (fs.exists("/logs") == false)
        createDir(fs, "/logs");
}

#endif