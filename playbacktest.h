#ifndef MACRO_H
#define MACRO_H

const char *LETTERS = "abcdefghijklmnopqrstuvwxyz";
const char *NUMBERS = "1234567890";
const char *VALID = "abcdefghijklmnopqrstuvwxyz1234567890W";
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
bool verifyC(char c);
char* getType(char *c, bool &value);
void wait(char *line);

/*
This is the only function that the user should ever have to use during the course of usage of this library.
This function is what actually starts reading the file and playing it back.
*/
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
				writeDebugStreamLine(lines[line]);
				line++;
				pos = 0;
				verified = false;
			}
			} else {
			if (!verified && pos == 0) {
				verified = verifyC(incoming);
			}
			if (verified) {
				lines[line][pos] = incoming;
				pos++;
			}
		}
	}

	//process the file
	char *type = "", *oldtype = "";
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
					toUse = getMotor(lines[i][0]);
					} else {
					toUse = getServo(lines[i][0]);
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

/*
This function checks a given character and checks to see if it is valid as according to the list of characters that it is allowed to read.
*/
bool verifyC(char c) {
	for (int i = 0; i < strlen(VALID); i++) {
		if (c == VALID[i]) {
			return true;
		}
	}
	return false;
	/* old code
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
	*/
}

/*
This function gets a motor based on the list of valid motor names and returns the motor's index for use.
*/
int getMotor(char c) {
	for (int i = 0; i < 26; i++) {
		if (c == LETTERS[i]) {
			return i;
		}
	}
	return -1;
}

/*
This function gets a servo based on the list of valid servo names and returns the servo's index for use.
*/
int getServo(char c) { 
	for (int i = 0; i < 10; i++) {
		if (c == NUMBERS[i]) {
			return i;
		}
	}
	return -1;
}

/*
This function gets a number from a string and returns it for use as an integer.
*/
int getNumber(char *c) {
	return atoi(c);
}

/*
This function checks a given string and compares it with valid values to see what type of command it is.
*/
char* getType(char *c, bool &value) {
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
	return "";
}

/*
This function waits a number of milliseconds based on the string supplied to it, formatted something like "W100," which would wait for 500 milliseconds.
*/
void wait(char *line) {
	int len = strlen(line);
	char *characters;
	for (int i = 1; i <= len; i++) {
		characters[i-1] = line[i];
	}
	int wTime = atoi(characters);
	wait1Msec(wTime);
}
