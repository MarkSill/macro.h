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
int getMotor(char c);
int getServo(char c);
int getNumber(char *c);
bool verify(char c);
string getType(char *ca, bool &value);

void playback(char *filename) {
    int size;
    char *lines[MAX_LINES];
    bool hasContent[MAX_LINES];
    int line = 0;
    OpenRead(handle, result, filename, size);
    sprintf(msg, "File size: %d bytes", size);
    writeDebugStream(msg);
    sprintf(msg, "File result: %d", result);
    writeDebugStream(msg);
    
    char incoming;
    int pos = 0;
    bool verified = false;
    
    //read the file
    for (int i = 0; i < size; i++) {
        ReadByte(handle, result, incoming);
        if (incoming == CR || incoming == LF) {
            if (incoming == LF) {
                hasContent[line] = verified;
                line++;
                pos = 0;
                verified = false;
            }
        } else {
            if (!verified && pos == 0) {
                verified = verify(incoming);
            }
            if (verified) {
                lines[line][pos] = incoming;
                pos++;
            }
        }
    }
    
    //process the file
    string type = "", oldtype = "";
    int toUse;
    bool value;
    for (int i = 0; i < MAX_LINES; i++) {
        if (hasContent[i]) { //make sure the line actually contains commands
            type = getType(lines[i], value);
            if (type == "wait") {
                wait(lines[i]);
                continue;
            } else if (type == "motor" || type == "servo") {
                oldtype = type;
                if (type == "motor") {
                    toUse = getMotor(lines[i]);
                } else {
                    toUse = getServo(lines[i]);
                }
            } else if (type == "value") {
                if (oldtype == "motor") {
                    motor[toUse] = getNumber(lines[i]);
                } else if (oldtype == "servo") {
                    servo[toUse] = getNumber(lines[i]);
                } else {
                    //do nothing
                }
            }
        }
        wait1Msec(1);
    }
}

bool verify(char c) {
    if (c == 'W') {
        return true;
    }
    for (int i = 0; i < 26; i++) {
        if (c == LETTERS[i]) {
            return true;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (c == NUMBERS[i]) {
            return true;
        }
    }
    return false;
}

int getMotor(char c) {
    for (int i = 0; i < 26; i++) {
        if (c == LETTERS[i]) {
            return i;
        }
    }
    return -1;
}

int getServo(char c) { 
    for (int i = 0; i < 10; i++) {
        if (c == NUMBERS[i]) {
            return i;
        }
    }
    return -1;
}

int getNumber(char *c) {
    return atoi(c);
}

string getType(char *c, bool &value) {
    if (c[0] == 'W') {
        value = false;
        return "wait";
    }
    if (!value) {
        for (int i = 0; i < 26; i++) {
            if (c[0] == LETTERS[i]) {
                value = true;
                return "motor";
            }
        }
        for (int i = 0; i < 10; i++) {
            if (c[0] == NUMBERS[i]) {
                value = true;
                return "servo";
            }
        }
    } else {
        for (int i = 0; i < 10; i++) {
            if (c[0] == NUMBERS[i]) {
                value = false;
                return "value";
            }
        }
    }
}
