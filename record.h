/*
macro.h was originally created by Caleb Jeppesen.
This file was last edited on 1/10/15.
This file is licensed under MIT.
*/

//make sure no errors arise from constant conflicts
#ifndef MACRO_H
#define MACRO_H

//constants
const int MAX_SIZE = 12;
const int MAX_STEPS = 30000;

const char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const char numbers[10] = {'1','2','3','4','5','6','7','8','9','0'};
const char *endl = "\r\n";

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

int step = 0, startMotor, startServo, endMotor, endServo;

void startRecording(int startMtr, int startSrv, int endMtr, int endSrv, char *filename) {
	startMotor = startMtr;
	startServo = startSrv;
	endMotor = endMtr;
	endServo = endSrv;
	int size = 1000;
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
			write(letters[i], motor[i]);
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
	step++;
	if (step >= MAX_STEPS) {
		recording = false;
	}
	wait1Msec(1);
}

void write(char name, int value) {
	string test = "0";
	sprintf(test, "%c", name);
	writeDebugStreamLine(test);
	WriteString(handle, result, test);
	WriteString(handle, result, endl);
	test = "0";
	sprintf(test, "%d", value);
	writeDebugStreamLine(test);
	WriteString(handle, result, test);
	WriteString(handle, result, endl);
}

void stopRecording() {
	Close(handle, result);
	string test = "-1";
	sprintf(test, "%d", result);
	writeDebugStreamLine(test);
}
