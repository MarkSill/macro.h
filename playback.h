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

TFileHandle handle;
TFileIOResult result;

typedef struct {
	char array[5];
} arrayChar;

#endif

int step = 0;

void playback(char *filename, int startMtr, int startSrv, int endMtr, int endSrv);
int getNumber(arrayChar line);

void playback(char *filename, int startMtr, int startSrv, int endMtr, int endSrv) {
	int size;
	int msize = 32;
	char lines[32][5];
	char incoming;
	int line = 0, int pos = 0;
	OpenRead(handle, result, filename, size);
	bool isValue = false;
	
	for (int i = 0; i < size; i++) {
		ReadByte(handle, result, incoming);
		if (incoming == LF || incoming == CR) {
			if (incoming == LF) {
				line++;
				pos = 0;
			}
		} else {
			lines[i][pos] = incoming;
			pos++;
			string h = "";
			sprintf(h, "%c", incoming);
			writeDebugStream(h);
		}
	}
	Close(handle, result);
	string rslt = "";
	sprintf(rslt, "Result: %d", result);
	writeDebugStreamLine(rslt);
	for (int i = 0; i < msize; i++) {
		writeDebugStreamLine(lines[i]);
	}
	
	//actual processing
	bool isLetter = false;
	int mode = 0;
	int waiting = 0;
	for (int lne = 0; lne < msize; lne++) {
		char line[5] = lines[lne];
		for (int ltr = 0; ltr < 26; ltr++) { //check to see if it's a letter
			if (letters[ltr] == line[0]) {
				isLetter = true;
				mode = ltr;
				continue;
			}
		}
		//check first to see if we should wait
		if (line[0] == 'W') {
			arrayChar arry;
			arry.array = line;
			wait1Msec(getNumber(arry));
			continue;
		}
		//if not a letter, we arrive here at last!
		arrayChar arry;
		arry.array = line;
		
	}
}

int getNumber(arrayChar line) {
	//needs to use atoi
	char *lne = "";
	for (int i = 0; i < 5; i++) {
		for (int n = 0; n < 10; n++) {
			if (line[i] == numbers[n]) {
				lne += line[i];
				break;
			}
		}
	}
	return atoi(lne);
}
