/*
macro.h was originally created by Caleb Jeppesen.
This file was last edited on 1/29/15.
This file is licensed under MIT.
*/

/* TODO:
 * Add full servo support. This shouldn't be all that difficult to add.
 * Making saving more efficient. Currently there are a lot of empty spaces that are eating up available space. We are very limited, so every byte counts!
 * Figure out what on Earth is going on with the saving system. It really likes storing characters from previous saving sessions for some reason, and I can't figure out how to clear the strings, even though they are being reinitialized every time they go through the functions/loops.
 * Update system to save everything through bytes. This has a lot to do with efficiency. Using WriteFloat seems to be the cause of the empty spaces.
 * Currently system saves like this: d;50  ;W5000  ;d0  ; (21 bytes). An even better way would be this: d50W5000d0 (10 bytes). This last one may be a little optimistic though. It'll take a bit of work to get working, mainly rewrites from current code that is being used.
*/

/* Notes on saving most efficiently:
In order to save in the extremely efficient way mentioned above in the TODO section, the library will need a major rewrite. More booleans, more states, more stuff (although I may be able to reduce the actual library's size, so that'll be useful if I can manage to do that).
Example comparisions:
	Current: d;50  ;W5000  ;d;0  ; (21 bytes)
	Soon-to-be: d50W5000d0 (10 bytes)

This may not seem like a big deal, but this small increase is massively scaled when you have larger files. For example, take the largest possible file: Lots of servos all moving to negative three digit values: 7 bytes per command! This amounts to 210000 bytes in a file!
Now let's take the other format, which is only 6 bytes per command: Merely 180000 bytes, or a 30000 byte decrease, which amounts to nearly 30 kilobytes of space saved! That's around a third of the NXT's memory, so it's extremely important that we go through with this major change. It will not only save space, but will also make the library faster and more efficient, as it will have to loop through less characters when compared to the current system. Definitely only benefits!
*/

//make sure no errors arise from constant conflicts
#ifndef MACRO_H
#define MACRO_H

//constants
const int MAX_SIZE = 12;
const int MAX_STEPS = 30000;

const char LETTERS[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const char NUMBERS[10] = {'1','2','3','4','5','6','7','8','9','0'};
//const byte NEGATIVE = '-'; //not actually needed because WriteFloat includes the negative sign
const byte WAIT = 'W';
//const byte SERVO = 'S'; //not currently being used as the library does not currently support servos
const byte ENDL = ';';

#endif

//functions
void startRecording(int startMtr, int startSrv, int endMtr, int endSrv, char *filename);
void clear();
void update();
void write(char name, int value);
void stopRecording();

bool recording = true;

//variables
TFileHandle handle;
TFileIOResult result;

int lastMotors[MAX_SIZE], lastServos[MAX_SIZE];

int step = 0, since = 0, startMotor, startServo, endMotor, endServo;

void startRecording(int startMtr, int startSrv, int endMtr, int endSrv, char *filename) {
	startMotor = startMtr;
	startServo = startSrv;
	endMotor = endMtr;
	endServo = endSrv;
	short size = 10000;
	Delete(filename, result);
	OpenWrite(handle, result, filename, size);
}

void clear() {
	for (int i = startMotor; i < endMotor; i++) {
		lastMotors[i] = 0;
	}
	for (int i = startMotor; i < endMotor; i++) {
		lastServos[i] = 0;
	}
}

void update() {
	bool changed = false;
	for (int i = startMotor; i <= endMotor; i++) {
		if (motor[i] != lastMotors[i]) {
			write(LETTERS[i], motor[i]);
			lastMotors[i] = motor[i];
			changed = true;
		}
	}
	/*for (int i = startServo; i <= endServo; i++) {
		if (servo[i] != lastServos[i]) {
			write(numbers[i], servo[servos[i]]);
			lastServos[i] = servo[servos[i]];
		}
	}*/
	if (changed) {
		if (since > 0) {
			WriteByte(handle, result, WAIT);
			string num = since;
			WriteString(handle, result, num);
			WriteByte(handle, result, ENDL);
		}
		since = 0;
	} else {
		since++;
	}
	step++;
	if (step >= MAX_STEPS) {
		recording = false;
	}
	wait1Msec(1);
}

void write(char name, int value) {
	//need to convert value to string so I can write bytes instead of strings
	string num = value;
	//sprintf(num, "%d", value);
	writeDebugStreamLine(num);
	WriteByte(handle, result, name);
	WriteByte(handle, result, ENDL);
	WriteString(handle, result, num);
	WriteString(handle, result, "");
	WriteByte(handle, result, ENDL);
}

void stopRecording() {
	Close(handle, result);
	string test = "error during sprintf()";
	sprintf(test, "%d", result);
	writeDebugStreamLine(test);
}
