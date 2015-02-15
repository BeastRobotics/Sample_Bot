/*
 * Grabber.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Beasty
 */
#include "WPILib.h"
#include "XboxController.h"
#include "IControl.h"

#define GRABBERCHANNELOPEN 0
#define GRABBERCHANNELCLOSE 1

#define EXTEND_TIME_IN_MS 500
#define EXTEND_COUNT EXTEND_TIME_IN_MS/5

class GrabberControl: public IControl {
	DoubleSolenoid *sol1;
	XboxController *xbox;
	int currentAutoCommand;
	int autoCount;

public:
	GrabberControl() {
		sol1 = new DoubleSolenoid(GRABBERCHANNELOPEN, GRABBERCHANNELCLOSE);
		xbox = XboxController::getInstance();
		currentAutoCommand = -1;
		autoCount = 0;
	}

	void AutonomousInit() {
		currentAutoCommand = -1;
		autoCount = 0;
	}

	int AutonomousPeriodic(int input) {
		int option = input;

		if (currentAutoCommand != option) {
			currentAutoCommand = option;
			autoCount = EXTEND_COUNT;
		}

		autoCount--;
		if (autoCount <= 0) {
			AutonomousInit();
			return 1;
		}

		switch (option) {
		case 0:
			SetGrabberArm(true);
			break;
		case 1:
			SetGrabberArm(false);
			break;

		}
		return 0;
	}

private:
	void TeleopPeriodic() {
		if (xbox->isRBumperHeld()) {
			SetGrabberArm(true);
		} else {
			SetGrabberArm(false);
		}
	}

	void SetGrabberArm(bool isExtended) {
		if (isExtended) {
			sol1->Set(sol1->kForward);
		} else {
			sol1->Set(sol1->kReverse);
		}
	}
};

