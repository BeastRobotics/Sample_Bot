/*
 * ArcadeDrive.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Beasty
 */
#include "WPILib.h"
#include "IControl.h"
#include "XboxController.h"

class ArcadeDrive: public IControl {
	XboxController *xbox;
	RobotDrive myRobot;

public:
	ArcadeDrive():myRobot(0, 1) {
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance();
	}
	void TeleopPeriodic() {
		double x = xbox->getAxisLeftX();
		double y = (-1) * xbox->getAxisLeftY();

		SmartDashboard::PutNumber("X Value", x);
		SmartDashboard::PutNumber("Y Value", y);

		myRobot.ArcadeDrive(y,x);
	}

};



