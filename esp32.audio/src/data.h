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


// Initialize the File System for read write
void init_filesystem(fs::FS &fs)
{
    // Cleaning up first
    if (fs.exists("/inbox") == true)
    {
        Serial.println("Removing inbox Directory");
        removeDir(fs, "/inbox");
    }

    if (fs.exists("/logs") == true)
    {
        Serial.println("Removing logs Directory");
        removeDir(fs, "/logs");
    }
    if (fs.exists("/system") == true)
    {
        Serial.println("Removing system Directory");
        removeDir(fs, "/system");
    }

    if (SD.exists("/inbox") == false)
    {
        Serial.println("Creating Inbox Directory");
        createDir(fs, "/inbox");
    }

    if (fs.exists("/system") == false)
    {
        Serial.println("Create System Directory");
        createDir(fs, "/system");
    }
    if (fs.exists("/logs") == false)
    {
        Serial.println("Create Logs Directory");
        createDir(fs, "/logs");
    }

}

#endif