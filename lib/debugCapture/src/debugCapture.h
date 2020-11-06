/*
 * File: debugCapture.h
 * Project: src
 * File Created: Saturday, 31st October 2020 1:32:48 pm
 * Author: Matthew Bennett (matthew@mlbennett.com)
 * -----
 * Last Modified: Saturday, 31st October 2020 1:57:15 pm
 * Modified By: Matthew Bennett (matthew@mlbennett.com>)
 * -----
 * Copyright <<projectCreationYear>> - 2020 Your Company
 */



/* Required methods
 * Create File
 * Close File
 * Next File
 * Add Debug Info
 * List Files
 * Read File
 * Delete File
 * Disk Status
 */

#ifndef DEBUGCAPTURE_H
#define DEBUGCAPTURE_H

#define     LIST_SEPERATOR      ';'


class debugCapture {
    public:
        debugCapture();
        int createFile();
        bool nextFile();
        bool addDebugInfo(char* debugInfo, int infoLength);
        bool listFiles(char* fileList);
        bool readLine(char* debugLine);
        bool deleteFile();
        bool diskStatus();
    private:
        const int   fileFILESIZE = 256;
        char        currentFILENAME[20];
        const char  *baseFILENAME = "debugFile_";
        const char  *listSEPERATOR = ";";

        bool flash_spi_initialise();
        bool flash_fileexists(char *filename);
};

#endif /* DEBUGCAPTURE_H*/
