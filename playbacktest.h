#ifndef MACRO_H
#define MACRO_H

const char *LETTERS = "abcdefghijklmnopqrstuvwxyz";
const char *NUMBERS = "1234567890";
const int MAX_LINES = 64;

const byte CR = 13;
const byte LF = 10;

TFileHandle handle;
TFileIOResult result;

#endif

string msg = "";

void playback(char *filename);

void playback(char *filename) {
    int size;
    char *lines[MAX_LINES];
    int line = 0;
    OpenRead(handle, result, filename, size);
    sprintf(msg, "File size: %d bytes", size);
    writeDebugStream(msg);
    sprintf(msg, "File result: %d", result);
    writeDebugStream(msg);
    
    char incoming;
    
    //read the file
    for (int i = 0; i < size; i++) {
        ReadByte(handle, result, incoming);
        if (incoming == CR || incoming == LF) {
            if (incoming == LF) {
                line++;
            }
        } else {
            
        }
    }
}
