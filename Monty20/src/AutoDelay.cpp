/*
 * AutoDelay.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: Beast Laptop
 */

#include <AutoDelay.h>
#define LENGTH_BETWEEN_CALLS 20

AutoDelay::AutoDelay() {
	// TODO Auto-generated constructor stub
	autoCount = 0;
	prevCommand = -1;
}

AutoDelay::~AutoDelay() {
	// TODO Auto-generated destructor stub
}

void AutoDelay::AutonomousInit() {
	autoCount = 0;
	prevCommand = -1;
}

//input is in ms
int AutoDelay::AutonomousPeriodic(int input) {
	if (input != prevCommand) {
		autoCount = input / LENGTH_BETWEEN_CALLS;
		prevCommand = input;
	}

	autoCount--;
	if (autoCount <= 0) {
		AutonomousInit();
		return 1;
	}

	return 0;
}
