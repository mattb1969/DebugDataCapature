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



debugCapture::debugCapture() {
// Initalise variables
bool status = false;

flash_spi_initialise();

}

int debugCapture::createFile(int fileSize, char* fileName) {
       char        header[20];
    char        contents[21];
    char        filename[20]; //*filename;
    int         filesize = 256;
    int         filecounter = 1;
    int         status = false;
    char        filename_extension[10];
    
    Serial.println("Creating file");
    strcpy(header,"Debug File\n");
    strcpy(contents,"Some contents here!\n");
    
    while (!status) {
        Serial.println(".");
        strcpy(filename,"");
        strcat(filename,"debugFile_");
        itoa(filecounter, filename_extension, 10);
        strcat(filename, filename_extension);
        //filename = "debugFile_"+(char*)filecounter;

        if (flash_fileexists(filename)) {
            if (DEBUG) Serial.println("Found the file, trying a new number");
            filecounter ++;
        }
        else {
            if (DEBUG) Serial.println("File name selected");
            status = true;
        }
        if (filecounter > 1000) {
            if (DEBUG) Serial.println("Counter exceeded 1000");
            blink_led(500,500,7);
        }
    }
    Serial.println("File being used");
    Serial.println(filename);

    SerialFlash.opendir();
    //return true;
    // create the file on the Flash chip and copy data
    if (SerialFlash.create(filename, filesize)) {
        Serial.println("Able to create file");
        
        status = true;
    } else {
        Serial.println("  unable to create file");
    }

    return status;

}
}

bool debugCapture::closeFile() {

}

bool debugCapture::nextFile() {

}

bool debugCapture::addDebugInfo(char* debugInfo) {
    // rest of the info is in createfile
       ff = SerialFlash.open(filename);                            // This needs to be moved to seperate function
        if (ff) {
            ff.write(header, 11);
            ff.write(contents, 20);
            ff.close();
        } else {
            Serial.println("  error opening freshly created file!");
        }
}

bool debugCapture::listFiles(char* fileList) {

    int status = false;
    if (DEBUG) Serial.println("Reading Flash Files");
    SerialFlash.opendir();
    while (!status) {
        char filename[64];
        uint32_t filesize;

        if (SerialFlash.readdir(filename, sizeof (filename), filesize)) {
            if (DEBUG) {
                Serial.print("Filename:  ");
                Serial.print(filename);
                Serial.print(" of size ");
                Serial.print(filesize);
                Serial.print(" bytes");
                Serial.println();
            }
            flash_readfile(filename, filesize);
        } else {
            Serial.println("No More Files to read!");
            status = true; // no more files, completed reading flash
        }
    }
    return status;
}

bool debugCapture::openFile() {

}

bool debugCapture::readLine(char* debugLine) {
//bool flash_readfile(char filename[64], int filesize) {
    
    char    buffer[filesize];
    bool    status = false;
    
    Serial.println("File being read:");
    Serial.print("=== ");
    Serial.print(filename);
    Serial.println(" ===");

    //return true;
    // create the file on the Flash chip and copy data
    if (ff = SerialFlash.open(filename)) {
        ff.read(buffer, filesize);
        Serial.println(buffer);

        ff.close();
        Serial.println("=== end of file ===");
        status = true;

    } else {
        Serial.println("  error opening file!");
    }

    return status;
}

bool debugCapture::deleteFile() {

}

bool debugCapture::diskStatus() {
 
}

bool debugCapture::flash_spi_initialise() {
    int status = false;
    if (DEBUG) Serial.println("Initialising the Flash");
    digitalWrite(LORA_RESET, LOW);
    status = SerialFlash.begin(SPI1, FLASH_CS);
    if (!status) {
        if (DEBUG) Serial.println("Unable to 'begin' the Serial flash");
    }
    uint8_t id[5];
    SerialFlash.readID(id);
    status = SerialFlash.ready();
    return status;
}