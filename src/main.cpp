/*
 * Program to develop Serial Flash comms with LoRaWan
 * 
 * Program Captures data regularly
 * 
 * Each run through main loop a new file is created
 * ON start all the files in the flash are printed to console
 *  - option to delete files
 * Old data is removed once the flash is full
 * Information captured
 * - Values for sending
 * - Status codes
 * - Textual information
 * 
 * Notes:
 * FLASH_CS to define flash pin
 * 
 */


#include <MKRWAN.h>
#include <SerialFlash.h>
#include <SPI.h>
#include "debugCapture.h"


volatile bool rtc_triggered = false;

static const uint8_t rtc_timer = 15;
static const uint8_t TESTMODE_SET = 1;

volatile bool DEBUG = true;
volatile bool TEST_MODE = false;

volatile uint8_t rtccounter = 5;

SerialFlashFile ff;

void blink_led(int ontime = 100, int offtime = 50, int flashes = 1) {
    while (flashes > 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        delayMicroseconds(ontime * 1000);
        digitalWrite(LED_BUILTIN, LOW);
        if (flashes > 1) delayMicroseconds(offtime * 1000); // If it is not the last flash, wait until returning to the top of the loop
        flashes--;
    }
    return;
}

bool flash_spi_initialise() {
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

bool flash_readfile(char filename[64], int filesize) {
    
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

bool flash_readfilenames() {
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

bool flash_fileexists(char *filename) {
    // Check if the file exists, return true
    if (DEBUG) Serial.println("Checking if file exists");
    if (SerialFlash.exists(filename)) {
        return true;
    } else {
        return false;
    }
}
bool flash_createfile() {
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
        
        ff = SerialFlash.open(filename);
        if (ff) {
            ff.write(header, 11);
            ff.write(contents, 20);
            ff.close();
        } else {
            Serial.println("  error opening freshly created file!");
        }
    } else {
        Serial.println("  unable to create file");
    }

    return status;

}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(TESTMODE_SET, INPUT_PULLDOWN);
    pinMode(LORA_RESET, OUTPUT);
    
    char inputBuffer[20]; // Make whatever size you need plus one
    int charsReceived;

    digitalWrite(LORA_RESET, LOW);
    
    Serial.begin(9600);
    if (!Serial) delayMicroseconds(2500 * 1000);

    Serial.println("Debug Data Capture Test");
    blink_led(50, 450, 5);

    if (digitalRead(TESTMODE_SET)) {
        TEST_MODE = true;
    }
    else {
        // Not in test mode, so turn off power requirements.
        //Firstly ensure we are not running any debug software
        //DEBUG = false;
        //USBDevice.detach();

    }

    Serial.println("\nSerial Flash test with LoRa, Watchdog and RTC!");

    flash_spi_initialise();

    if (flash_readfilenames()) {
        Serial.println("Do you want to delete the files?");

        if (Serial.available()) {
            charsReceived = Serial.readBytesUntil('\n', inputBuffer, sizeof (inputBuffer) - 1); // Save room for NULL
            inputBuffer[charsReceived] = 0; // Make it a string
            Serial.print("there were ");
            Serial.print(charsReceived);
            Serial.print(" chars received, which are: ");
            Serial.println(inputBuffer);
        }
        SerialFlash.eraseAll();
    };

}

void loop() {

    blink_led(500,500);
    //delay(1000);
    if (Serial) Serial.println("Starting main loop and going to sleep");

    delayMicroseconds(15 * 1000 * 1000);

    flash_createfile();

}