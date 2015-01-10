/*
macro.h was originally created by Caleb Jeppesen.
This file was last edited on 1/10/15.
This file is licensed under MIT.
*/

//make sure no errors arise
#ifndef MACRO_H
#define MACRO_H

//constants
const int MAX_SIZE = 8;
const int MAX_STEPS = 30000;

const char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const char numbers[10] = {'1','2','3','4','5','6','7','8','9','0'};

const byte CR = 13;
const byte LF = 10;

#endif

TFileHandle handle;
TFileIOResult result;

void playback(char *filename, int startMtr, int startSrv, int endMtr, int endSrv);

void playback(char *filename, int startMtr, int startSrv, int endMtr, int endSrv) {
	int size;
	int msize = 32;
	char *lines = "";
	char incoming;
	int line = 0;
	OpenRead(handle, result, filename, size);
	bool isValue = false;
	
	for (int i = 0; i < size; i++) {
		ReadByte(handle, result, incoming);
		if (incoming == LF || incoming == CR) {
			if (incoming == LF)
				line++;
		} else {
			lines += incoming;
			string h = "";
			sprintf(h, "%c", incoming);
			writeDebugStream(h);
		}
	}
	writeDebugStreamLine(lines);
	Close(handle, result);
	string test = "";
	sprintf(test, "%d", result);
	writeDebugStreamLine(test);
	for (int i = 0; i < msize; i++) {
		//writeDebugStreamLine(lines[i]);
	}
}
