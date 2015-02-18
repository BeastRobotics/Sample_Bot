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
#define DAVIDS_FUN_INPUT 20
#define FINAL_DEBOUNCE_TURN 300

class MecanumDrive: public IControl {
protected:
	XboxController *xbox;
	RobotDrive* myRobot;
	Gyro *gyro;
	MultiOutputPID *motorOutput;
	Talon *motor1, *motor2, *motor3, *motor4;
	PIDController *autoRotateController;

	const static int frontLeftChannel = 2;
	const static int rearLeftChannel = 4;
	const static int frontRightChannel = 1;
	const static int rearRightChannel = 3;
	const static int gyroChannel = 0;
	int lastCommand;
	int lastCommandTurn;
	int lastCommandDrive;
	int autoDriveCounter;
	int autoTurnCounter;

	float x;
	float y;
	float twist;
	float angle;
	float speedFactor;
	float rotateSpeedFactor;
	float strafeSpeedFactor;
	bool creepMode;

public:
	MecanumDrive() {
		motor1 = new Talon(frontRightChannel);
		motor2 = new Talon(rearRightChannel);
		motor3 = new Talon(frontLeftChannel);
		motor4 = new Talon(rearLeftChannel);

		motorOutput = new MultiOutputPID(motor1, motor3, motor2, motor4, true);
		gyro = new Gyro(gyroChannel);
		autoRotateController = new PIDController(0.005, 0.0, 0.0, gyro,
				motorOutput);

		myRobot = new RobotDrive(motor3, motor4, motor1, motor2);
		myRobot->SetExpiration(0.1);
		xbox = XboxController::getInstance();
		myRobot->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true); // invert the left side motors
		myRobot->SetInvertedMotor(RobotDrive::kRearLeftMotor, true); // you may need to change or remove this to match your robot
		x = 0.0;
		y = 0.0;
		twist = 0.0;
		angle = 0.0;
		speedFactor = 1.0;
		rotateSpeedFactor = 0.5;
		strafeSpeedFactor = 0.5;

		lastCommand = 0;
		lastCommandTurn = 0;
		lastCommandDrive = 0;
		autoDriveCounter = 0;
		autoTurnCounter = 0;

		creepMode = false;
	}

	void AutonomousInit() {
		lastCommand = 0;
		lastCommandTurn = 0;
		lastCommandDrive = 0;
		autoDriveCounter = 0;
		autoTurnCounter = 0;
		autoRotateController->Disable();
		gyro->Reset();
		motorOutput->DisableOverDrive();
	}

	int AutonomousPeriodic(int input) {
		if (lastCommand != input) {
			lastCommand = input;
		}

		switch (input) {
		case 0:
			break;
		case 1:
			return turn(90);
		case 2:
			return turn(-90);
		case 3:
			return disableStuff();
		default:
			if (abs(input) > 10) {
				return drive(abs(input / DAVIDS_FUN_INPUT), input > 0);
			}
			break;
		}
		return 0;
	}
	int disableStuff() {
		AutonomousInit();
		return 0;
	}
	int turn(int input) {
		motorOutput->DisableOverDrive();
		if (lastCommandTurn != input) {
			lastCommandTurn = input;
			autoRotateController->SetSetpoint(input);
			autoRotateController->Enable();
			autoTurnCounter = FINAL_DEBOUNCE_TURN / DAVIDS_FUN_INPUT;
		}
		double gyroValue = gyro->GetAngle();
		if (abs(gyroValue - input) < THRESHHOLD_RANGE) {
			autoTurnCounter--;
			if (autoTurnCounter <= 0) {
				AutonomousInit();
				return 1;
			}
		} else {
			autoTurnCounter = FINAL_DEBOUNCE_TURN / DAVIDS_FUN_INPUT;
		}
		return 0;
	}

	int drive(int input, bool forward) { //input is how long we want to drive
		if (lastCommandDrive != input) {
			lastCommandDrive = input;
			motorOutput->SetOverDrive(forward ? 0.5 : -0.5);
			gyro->Reset();
			autoRotateController->SetSetpoint(0);
			autoRotateController->Enable();
			autoDriveCounter = input;
		}
		autoDriveCounter--;
		if (autoDriveCounter <= 0) {
			AutonomousInit();
			return 1;
		}
		return 0;
	}

	void creepModeSet() {
		if (xbox->isRightTriggerHeld()) {
			creepMode = true;
		}
	}

	void TeleopInit() {
		myRobot->SetSafetyEnabled(false);
		SmartDashboard::PutBoolean("Use Gyro", false);
		SmartDashboard::PutNumber("Gyro Direction", gyro->GetAngle());
		SmartDashboard::PutNumber("Y Speed Factor", 1.0);
		SmartDashboard::PutNumber("Rotate Speed Factor", 0.5);
		SmartDashboard::PutNumber("Strafe Speed Factor", 0.5);

		SmartDashboard::PutBoolean("Creep Mode", false);

		gyro->Reset();

		autoRotateController->Disable();
	}

	void TeleopPeriodic() {
		creepModeSet();
		SmartDashboard::PutNumber("Gyro Direction", gyro->GetAngle());
		speedFactor = SmartDashboard::GetNumber("Y Speed Factor");
		rotateSpeedFactor = SmartDashboard::GetNumber("Rotate Speed Factor");
		strafeSpeedFactor = SmartDashboard::GetNumber("Strafe Speed Factor");

		SmartDashboard::PutBoolean("Creep Mode", creepMode);

		if (creepMode) {
			speedFactor = 0.2;
		}

		x = xbox->getAxisLeftX();
		y = xbox->getAxisLeftY();
		twist = xbox->getAxisRightX();

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

		x *= strafeSpeedFactor;
		y *= speedFactor;
		twist *= rotateSpeedFactor;

		myRobot->MecanumDrive_Cartesian(x, y, twist, angle);
	}

	void setAutoProgram(int s) {
		//autoProgram = s;
	}

	void AutonomousExecute() {
		if (!(lastCommand || lastCommandTurn || lastCommandDrive)) {
			motor1->PIDWrite(0.0);
			motor2->PIDWrite(0.0);
			motor3->PIDWrite(0.0);
			motor4->PIDWrite(0.0);
		}
	}
};

