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

class GrabberControl : public IControl {
	DoubleSolenoid *sol1;
	XboxController *xbox;

public:
	GrabberControl() {
		sol1 = new DoubleSolenoid(GRABBERCHANNELOPEN, GRABBERCHANNELCLOSE);
		xbox = XboxController::getInstance();
	}

private:
	void TeleopPeriodic(){
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



