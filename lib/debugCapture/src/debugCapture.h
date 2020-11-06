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

char        currentFilename[20];


class debugCapture {
    public:
        debugCapture();
        int createFile(int fileSize, char* fileName);
        bool closeFile();
        bool nextFile();
        bool addDebugInfo(char* debugInfo);
        bool listFiles(char* fileList);
        bool openFile();
        bool readLine(char* debugLine);
        bool deleteFile();
        bool diskStatus();
    private:
        bool flash_spi_initialise()
}

#endif /* DEBUGCAPTURE_H*/
