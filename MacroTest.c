#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop, reversed)

#include "macro.h/record.h"
#include "JoystickDriver.c"

const int CONTROLLER_THRESHOLD = 10;

//variables
int x1, x2, y1, y2;

task main()
{

	startRecording(3, 0, 8, 0, "macro1.txt");

	//receive input
	while(recording) //this is where driver-controlled period code goes
  {
    getJoystickSettings(joystick);

    //controls
    x1 = joystick.joy1_x1;
    x2 = joystick.joy1_x2;
    y1 = joystick.joy1_y1;
    y2 = joystick.joy1_y2;
    if (abs(x1) < CONTROLLER_THRESHOLD) {
    	x1 = 0;
    }
   	if (abs(x2) < CONTROLLER_THRESHOLD) {
   		x2 = 0;
   	}
   	if (abs(y1) < CONTROLLER_THRESHOLD) {
   		y1 = 0;
   	}
   	if (abs(y2) < CONTROLLER_THRESHOLD) {
   		y2 = 0;
   	}

   	//logic and stuff, also I have no idea if this'll work or not, so we'll see I suppose
   	motor[motorD] = y1;
   	motor[motorE] = y2;
   	motor[motorF] = y1;
   	motor[motorG] = y2;

   	if (joy1Btn(5)) {
   		motor[motorI] = 50;
   	} else if (joy1Btn(6)) {
   		motor[motorI] = -50;
   	} else {
   		motor[motorI] = 0;
   	}

   	//stop program
   	if (joy1Btn(9)) {
   		recording = false;
   	}

   	update();
  }

	stopRecording();

}
