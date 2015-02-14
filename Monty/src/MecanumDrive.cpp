/*
 * MecanumDrive.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: Beasty
 */

#include <Gyro.h>
#include <IControl.h>
#include <RobotDrive.h>
#include <SmartDashboard/SmartDashboard.h>
#include <XboxController.h>
#include <cmath>
#include "WPILib.h"

#include "MultiOutputPID.h"
#define THRESHHOLD_RANGE 2 //measured in degrees

class MecanumDrive: public IControl {
	XboxController *xbox;
	RobotDrive* myRobot;
	Gyro *gyro;
	MultiOutputPID *motorOutput;
	Talon *motor1, *motor2, *motor3, *motor4;
	PIDController *autoRotateController;

	const static int frontLeftChannel=2;
	const static int rearLeftChannel=4;
	const static int frontRightChannel=1;
	const static int rearRightChannel=3;
	const static int gyroChannel=0;
	int lastCommand;

	float x;
	float y;
	float twist;
	float angle;
	float speedFactor;

public:
	MecanumDrive() {
		motor1=new Talon(frontRightChannel);
		motor2=new Talon(rearRightChannel);
		motor3=new Talon(frontLeftChannel);
		motor4=new Talon(rearLeftChannel);

		motorOutput = new MultiOutputPID(motor1,motor2,motor4,motor3,true);
		gyro=new Gyro(gyroChannel);
		autoRotateController=new PIDController(0.5, 0.0, 0.0, gyro, motorOutput);

		myRobot = new RobotDrive(motor3, motor4, motor1, motor2);
		myRobot->SetExpiration(0.1);
		xbox=XboxController::getInstance();
		myRobot->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		myRobot->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);	// you may need to change or remove this to match your robot
		x=0.0;
		y=0.0;
		twist=0.0;
		angle=0.0;
		speedFactor=1.0;
	}

	void AutonomousInit() {
		lastCommand=0;
		autoRotateController->Disable();
		gyro->Reset();
	}

	int AutonomousPeriodic(int input) {
		if (lastCommand!=input) {
			lastCommand=input;
			autoRotateController->SetSetpoint(input);
			autoRotateController->Enable();
		}
		double gyroValue=gyro->GetAngle();
		if (abs(gyroValue-input)<THRESHHOLD_RANGE) {
			AutonomousInit();
			return 1;
		}
		return 0;
	}

	void TeleopInit() {
		myRobot->SetSafetyEnabled(false);
		SmartDashboard::PutBoolean("Use Gyro", false);

		SmartDashboard::PutNumber("Speed Factor", speedFactor);

		gyro->Reset();
	}
	void TeleopPeriodic() {
		speedFactor=SmartDashboard::GetNumber("Speed Factor");

		x=xbox->getAxisLeftX();
		y=xbox->getAxisLeftY();
		twist=xbox->getAxisRightX();

		if (fabs(x)<0.1) {
			x=0.0;
		}

		if (fabs(y)<0.1) {
			y=0.0;
		}

		if (fabs(twist)<0.1) {
			twist=0.0;
		}

		if (SmartDashboard::GetBoolean("Use Gyro")) {
			angle=gyro->GetAngle();
		}
		else {
			angle=0.0;
		}

		x*=speedFactor;
		y*=speedFactor;
		twist*=speedFactor;

		myRobot->MecanumDrive_Cartesian(x, y, twist, angle);
	}

	void setAutoProgram(int s) {
		//autoProgram = s;
	}

};

