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

debugCapture dc;

void blink_led(int ontime = 100, int offtime = 50, int flashes = 1)
{
    while (flashes > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delayMicroseconds(ontime * 1000);
        digitalWrite(LED_BUILTIN, LOW);
        if (flashes > 1)
            delayMicroseconds(offtime * 1000); // If it is not the last flash, wait until returning to the top of the loop
        flashes--;
    }
    return;
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(TESTMODE_SET, INPUT_PULLDOWN);
    pinMode(LORA_RESET, OUTPUT);

    char inputBuffer[20]; // Make whatever size you need plus one
    int charsReceived;

    digitalWrite(LORA_RESET, LOW);

    //Serial.begin(9600);
    if (!Serial)
        delayMicroseconds(2500 * 1000);

    Serial.println("Debug Data Capture Test");
    blink_led(50, 450, 5);

    if (digitalRead(TESTMODE_SET))
    {
        TEST_MODE = true;
    }
    else
    {
        // Not in test mode, so turn off power requirements.
        //Firstly ensure we are not running any debug software
        //DEBUG = false;
        //USBDevice.detach();
    }

    Serial.println("\nSerial Flash test with LoRa, Watchdog and RTC!");

    if (dc.initialise() == false)
    {
        Serial.println("Unable to initialise the flash, aborting");
        blink_led(100, 100, 100);
        while (false)
            ;
    }

    char *listOfFiles ;        //@mattb1969 this is initialising with debug_1\0 and not empty!
    listOfFiles = (char *) malloc(1); //sizeof(char));
    strcpy(listOfFiles,"\0");

    if (dc.listFiles(listOfFiles))
    {
        Serial.println("The files are:-");
        Serial.println(listOfFiles);
        Serial.println("Do you want to delete the files?");

        if (Serial.available())
        {
            charsReceived = Serial.readBytesUntil('\n', inputBuffer, sizeof(inputBuffer) - 1); // Save room for NULL
            inputBuffer[charsReceived] = 0;                                                    // Make it a string
            Serial.print("there were ");
            Serial.print(charsReceived);
            Serial.print(" chars received, which are: ");
            Serial.println(inputBuffer);
        }
        //todo #7 Need to delete the files if request

        int startOfFilename = 0;
        char *fileToDelete;
        char *character;

        for (u_int8_t i = 0; i<sizeof(listOfFiles); i++) {
            if (listOfFiles[i] = LIST_SEPERATOR) {
                if (i>0) {
                    // Delete the file
                    dc.deleteFile(fileToDelete);

                    // Reset everything
                    strcpy(fileToDelete,'\0');
                    startOfFilename = i +1;
                }
                else {
                    //Problem - found LIST_SEPERATOR on first pass.

                }
            }
            else {
                fileToDelete = (char *)malloc(i - startOfFilename);
                character = listOfFiles[i]);
                strncat(fileToDelete,character,1);
            }

        }
    };
};


void loop()
{

    blink_led(500, 500);
    //delay(1000);
    if (Serial)
        Serial.println("Starting main loop and going to sleep");

    delayMicroseconds(15 * 1000 * 1000);

    dc.createFile();
}