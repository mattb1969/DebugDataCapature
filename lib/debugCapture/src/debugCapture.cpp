/*
 * File: debugCapture.cpp
 * Project: src
 * File Created: Saturday, 31st October 2020 1:32:32 pm
 * Author: Matthew Bennett (matthew@mlbennett.com)
 * -----
 * Last Modified: Tuesday, 3rd November 2020 10:16:07 pm
 * Modified By: Matthew Bennett (matthew@mlbennett.com>)
 * -----
 * Copyright - 2020 
 * <<license>>
 */

#include "debugCapture.h"
#include <SerialFlash.h>
#include <SPI.h>

SerialFlashFile ff;

debugCapture::debugCapture()
{
    // Initalise variables

    flash_spi_initialise();
}

int debugCapture::createFile()
{

    char filename[20];
    int filecounter = 1;
    int status = false;
    char filename_extension[10];

    //Serial.println("Creating file");


    while (!status)
    {
        //Serial.println(".");
        strcpy(filename, "");
        strcat(filename, baseFILENAME);
        itoa(filecounter, filename_extension, 10);
        strcat(filename, filename_extension);

        if (flash_fileexists(filename))
        {
            //if (DEBUG) Serial.println("Found the file, trying a new number");
            filecounter++;
        }
        else
        {
            //if (DEBUG) Serial.println("File name selected");
            status = true;
        }
        //if (filecounter > 1000) {
        //if (DEBUG) Serial.println("Counter exceeded 1000");
        //blink_led(500,500,7);
        //todo: #1 need to set a maximum in case of error
        //}
    }
    //Serial.println("File being used");
    //Serial.println(filename);

    strcpy(currentFILENAME, filename);

    SerialFlash.opendir();
    //return true;
    // create the file on the Flash chip and copy data
    if (SerialFlash.create(filename, fileFILESIZE))
    {
        //Serial.println("Able to create file");

        status = true;
    }
    else
    {
        //Serial.println("  unable to create file");
        status = false;
    }

    return status;
}

bool debugCapture::nextFile()
{
    return true;
}

bool debugCapture::addDebugInfo(char *debugInfo, int infoLength)
{
    char header[20];
    strcpy(header, "Debug File\n");
    //todo #4 Need to change the header to be more releastic, ideally time or something
    // rest of the info is in createfile
    ff = SerialFlash.open(currentFILENAME); // This needs to be moved to seperate function
    if (ff)
    {
        ff.write(header, 11);
        ff.write(debugInfo, infoLength);
        ff.close();
    }
    else
    {
        Serial.println("  error opening freshly created file!");
    }
    return true;
    //todo #6 Need to actually set the bool status correctly
}

bool debugCapture::listFiles(char *fileList)
{

    int status = false;
    //if (DEBUG) Serial.println("Reading Flash Files");
    SerialFlash.opendir();
    while (!status)
    {
        char filename[64];
        uint32_t filesize;

        if (SerialFlash.readdir(filename, sizeof(filename), filesize))
        {
            //if (DEBUG) {
            //    Serial.print("Filename:  ");
            //    Serial.print(filename);
            //    Serial.print(" of size ");
            //    Serial.print(filesize);
            //    Serial.print(" bytes");
            //    Serial.println();
            //}
            strcpy(fileList, filename);
            strncpy(fileList, listSEPERATOR, 1);
        }
        else
        {
            //Serial.println("No More Files to read!");
            status = true; // no more files, completed reading flash
        }
    }

    //todo #2
    return status;
}

bool debugCapture::readLine(char *debugLine)
{
    //bool flash_readfile(char filename[64], int filesize) {

    char buffer[fileFILESIZE];
    bool status = false;

    //Serial.println("File being read:");
    //Serial.print("=== ");
    //Serial.print(filename);
    //Serial.println(" ===");

    //return true;
    // create the file on the Flash chip and copy data
    if (ff = SerialFlash.open(currentFILENAME))
    {
        ff.read(buffer, fileFILESIZE);
        Serial.println(buffer);

        ff.close();
        Serial.println("=== end of file ===");
        status = true;
    }
    else
    {
        Serial.println("  error opening file!");
    }

    return status;
}

bool debugCapture::deleteFile()
{
    return true;
}

bool debugCapture::diskStatus()
{
    return true;
}

bool debugCapture::flash_spi_initialise()
{
    int status = false;
    //if (DEBUG)
    //    Serial.println("Initialising the Flash");
    digitalWrite(LORA_RESET, LOW);
    status = SerialFlash.begin(SPI1, FLASH_CS);
    //if (!status)
    //{
    //    if (DEBUG)
    //        Serial.println("Unable to 'begin' the Serial flash");
    //}
    uint8_t id[5];
    SerialFlash.readID(id);
    status = SerialFlash.ready();
    return status;
}

bool debugCapture::flash_fileexists(char *filename)
{
    // Check if the file exists, return true
    //if (DEBUG)
    //    Serial.println("Checking if file exists");
    if (SerialFlash.exists(filename))
    {
        return true;
    }
    else
    {
        return false;
    }
}