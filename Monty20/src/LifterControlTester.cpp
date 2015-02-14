/*
 * LifterControlTester.cpp
 *
 *  Created on: Feb 12, 2015
 *      Author: Beasty
 */

#include "WPIlib.h"
#include "XboxController.h"

class LifterControlTester {
	Talon *lifter;
	XboxController *xbox;

	DigitalInput *UpperLimit;
	DigitalInput *LowerLimit;

public:
	LifterControlTester() {
		lifter = new Talon(5);
		xbox = XboxController::getInstance();
		UpperLimit = new DigitalInput(5);
		LowerLimit = new DigitalInput(4);
	}

	void init() {
		lifter->Set(0.0);
	}

	void move(){
		bool UpperLimitValue = UpperLimit->Get();
		bool LowerLimitValue = LowerLimit->Get();

		SmartDashboard::PutBoolean("Lower Limit 1", LowerLimitValue);
		SmartDashboard::PutBoolean("Upper Limit 1", UpperLimitValue);

		if (xbox->isBHeld()) {
			lifter->Set(0.5);
		} else if (xbox->isXHeld()) {
			lifter->Set(-0.5);
		} else {
			lifter->Set(0.0);
		}
	}
};


