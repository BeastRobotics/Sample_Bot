/*
 * MecanumDrive.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: Beasty
 */

#include "WPILib.h"
#include "IControl.h"
#include "XboxController.h"

class MecanumDrive: public IControl {
	XboxController *xbox;
	RobotDrive myRobot;
	Gyro *gyro;

	const static int frontLeftChannel = 2;
	const static int rearLeftChannel = 4;
	const static int frontRightChannel = 1;
	const static int rearRightChannel = 3;

	float x;
	float y;
	float twist;
	float angle;
	float speedFactor;

public:
	MecanumDrive() :
			myRobot(frontLeftChannel, rearLeftChannel, frontRightChannel,
					rearRightChannel) {
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance();
		x = 0.0;
		y = 0.0;
		twist = 0.0;
		angle = 0.0;
		speedFactor = 1.0;
		gyro = new Gyro(1);
	}

	void AutonomousInit() {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {
		myRobot.SetSafetyEnabled(false);
		SmartDashboard::PutBoolean("Use Gyro", false);

		SmartDashboard::PutNumber("Speed Factor", speedFactor);

		gyro->Reset();
	}
	void TeleopPeriodic() {
		SmartDashboard::PutNumber("Speed Factor", speedFactor);
		speedFactor = SmartDashboard::GetNumber("Speed Factor");

		x = -xbox->getAxisLeftX();
		y = xbox->getAxisLeftY();
		twist = -xbox->getAxisRightX();

		if (fabs(x) < 0.1) {
			x = 0.0;
		}

		if (fabs(y) < 0.1) {
			y = 0.0;
		}

		if (fabs(twist) < 0.1) {
			twist = 0.0;
		}

		if (SmartDashboard::GetBoolean("Use Gyro")) {
			angle = gyro->GetAngle();
		} else {
			angle = 0.0;
		}

		x *= speedFactor;
		y *= speedFactor;
		twist *= speedFactor;

		myRobot.MecanumDrive_Cartesian(x, y, twist, angle);
	}

	void setAutoProgram(int s) {
		//autoProgram = s;
	}

};

