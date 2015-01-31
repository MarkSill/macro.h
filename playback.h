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
const int LINE_SIZE = 8;

const char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const char numbers[10] = {'1','2','3','4','5','6','7','8','9','0'};

const byte CR = 13;
const byte LF = 10;

TFileHandle handle;
TFileIOResult result;

typedef struct {
	char array[LINE_SIZE];
	byte hello;
} arrayChar;

#endif

int step = 0;

void playback(char *filename, int startMtr, int startSrv, int endMtr, int endSrv);
int getNumber(arrayChar lines);

void playback(char *filename, int startMtr, int startSrv, int endMtr, int endSrv) {
	int size;
	const int msize = 64;
	char *lines[msize];
	char incoming;
	int line = 0;//, pos = 0;
	OpenRead(handle, result, filename, size);
	bool isValue = false;

	string t = "";
	sprintf(t, "File size is %d bytes", size);
	writeDebugStreamLine(t);

	for (int i = 0; i < msize; i++) {
		char *hi = "";
		lines[i] = hi;
	}

	for (int i = 0; i < size; i++) {
		ReadByte(handle, result, incoming);
		if (incoming == LF || incoming == CR) {
			if (incoming == LF) {
				line++;
				//pos = 0;
			}
		} else {
			char *empty = "";
			char *in = empty + incoming;
			strcat(lines[line], in); //I need to figure out how to add a char to a char * very badly!!
			/*if (pos <= LINE_SIZE-1) {
				lines[line] += incoming;
			} else {
				writeDebugStreamLine("pos >= LINE_SIZE!");
			}
			pos++;*/
		}
	}
	Close(handle, result);
	if (result != 0) {
		string rslt = "";
		sprintf(rslt, "ERROR: %d", result);
		writeDebugStreamLine(rslt);
	}

	//actual processing
	bool isLetter = false;
	int mde = 0;
	int waiting = 0;
	for (int lne = 0; lne < msize; lne++) {
		mde = 0;
		char *line;
		line = lines[lne];
		bool con = false;
		writeDebugStreamLine(line);
		/*
		for (int n = 0; n < LINE_SIZE; n++) {
			bool found = false;
			for (int i = 0; i < 26; i++) {
				if (lines[lne][n] == letters[i]) {
					string hi = "";
					sprintf(hi, "%c, %c", line[n], letters[i]);
					//writeDebugStreamLine(hi);
					found = true;
				}
			}
			for (int i = 0; i < 10; i++) {
				if (lines[lne][n] == numbers[i]) {
					found = true;
					string hi = "";
					sprintf(hi, "%c, %c", line[n], numbers[i]);
					writeDebugStreamLine(hi);
				}
			}
			if (lines[lne][n] == 'W') {
				//writeDebugStreamLine("W");
				found = true;
			}
			if (found) {
				//writeDebugStreamLine("Found!");
				line[n] = lines[lne][n];
			} else {
				line[n] = ' ';
			}
			con = !found;
		}
		if (con) {
			//writeDebugStreamLine("Continuing!");
			continue;
		}
		writeDebugStreamLine(line);
		for (int ltr = 0; ltr < 26; ltr++) { //check to see if it's a letter
			if (letters[ltr] == line[0]) {
				isLetter = true;
				mde = ltr;
				continue;
			}
		}
		arrayChar arry;
		arry.array = line;
		int n = getNumber(arry);
		//check first to see if we should wait
		bool hasW = false;
		for (int q = 0; q < LINE_SIZE; q++) {
			if (line[q] == 'W') {
				hasW = true;
				string h = "";
				sprintf(h, "%d", q);
				//writeDebugStreamLine(h);
			}
		}
		/*if (hasW) {
			writeDebugStream("Waiting for ");
			string tst = "";
			sprintf(tst, "%d", n);
			writeDebugStream(tst);
			writeDebugStreamLine(" milliseconds...");
			wait1Msec(n);
			continue;
		}*/
		if (mde > 0 && mde < 9) {
			//if not a letter, we arrive here at last!
			string tst = "";
			sprintf(tst, "Motor: %c", line[0]);
			//writeDebugStreamLine(tst);
			tst = "";
			sprintf(tst, "Value: %d", atoi(line));
			//writeDebugStreamLine(tst);
			//writeDebugStreamLine(line);
			//motor[mde] = n;
			wait1Msec(1);
		}
	}
	writeDebugStreamLine("Finished!");
}

//this should return a number from a character array (passed through the function as a struct since you can't use arrays in functions for whatever reason)
int getNumber(arrayChar lines) {
	char *llne = "";
	for (int i = 0; i < LINE_SIZE; i++) {
		char line[LINE_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8'};
		for (int n = 0; n < 10; n++) {
			if (line[i] == numbers[n]) {
				llne += line[i];
				string hi = "";
				sprintf(hi, "%c", line[i]);
				//writeDebugStream(hi);
				break;
			}
		}
	}
	//writeDebugStreamLine("");
	//writeDebugStreamLine(llne);
	int n = atoi(llne);
	string hi = "";
	sprintf(hi, "%d", n);
	//writeDebugStreamLine(hi);
	return n;
}
