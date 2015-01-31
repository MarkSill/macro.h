#ifndef MACRO_H
#define MACRO_H

const char *LETTERS = "abcdefghijklmnopqrstuvwxyz";
const char *NUMBERS = "1234567890";
const char *VALID = "abcdefghijklmnopqrstuvwxyz1234567890WS-";

TFileHandle handle;
TFileIOResult result;

#endif

string msg = "";

void playback(char *filename, int minimum, int maximum);
char * process(char *lines, bool hasContent, char *oldtype);
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
void playback(char *filename, int minimum, int maximum) {
	short size;
	char lines[6];
	bool hasContent;
	int line = 0;
	OpenRead(handle, result, filename, size);
	//sprintf(msg, "File size: %d bytes", size);
	writeDebugStreamLine(msg);
	//sprintf(msg, "File result: %d", result);
	writeDebugStreamLine(msg);

	char incoming;
	char *oldtype = "";
	int pos = 0;
	bool verified = false;

	//read the file
	for (int i = 0; i < size; i++) {
		ReadByte(handle, result, incoming);
		if (incoming == ';') {
			hasContent = verified;
			//writeDebugStreamLine(lines);
			line++;
			pos = 0;
			verified = false;
			//process the file
			oldtype = process(lines, hasContent, oldtype);
			for (int z = 0; z < 6; z++) {
				lines[z] = ' ';
			}
			} else {
			if (!verified && pos == 0) {
				verified = verifyC(incoming);
			}
			if (verified) {
				lines[pos] = incoming;
				} else {
				lines[pos] = ' ';
			}
			pos++;
		}
	}
	for (int i = minimum; i < maximum; i++) {
		motor[i] = 0;
	}
}

char * process(char *lines, bool hasContent, char *oldtype) {
	//process the file
	char *type = "";
	int toUse;
	bool value;
	if (hasContent) { //make sure the line actually contains commands
		type = getType(lines, value);
		if (type == "wait") {
			wait(lines);
		} else if (type == "motor" || type == "servo") {
			oldtype = type;
			if (type == "motor") {
				toUse = getMotor(lines[0]);
			} else if (type == "servo") {
				toUse = getServo(lines[0]);
			}
		} else if (type == "value") {
			if (oldtype == "motor") {
				motor[toUse] = getNumber(lines);
			} else if (oldtype == "servo") {
				servo[toUse] = getNumber(lines);
			} else {
				//do nothing
			}
		}
	}
	wait1Msec(1);
	return oldtype;
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
	//writeDebugStreamLine(c);
	return atoi(c);
}

/*
This function checks a given string and compares it with valid values to see what type of command it is.
*/
char * getType(char *c, bool &value) {
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
				//return "servo";
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
	int len = strlen(line), dex = 0;
	char *characters = "";
	for (int i = 1; i <= len; i++) {
		for (int z = 0; z < 10; z++) {
			if (NUMBERS[z] == line[i]) {
				characters[dex] = line[i];
				dex++;
			}
		}
	}
	int wTime = atoi(characters);
	wait1Msec(wTime);
}
