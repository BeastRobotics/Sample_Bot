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
#include <NewXboxController.h>
#include <cmath>
#include "WPILib.h"
#include "BeastSpeedControl.h"
#include <JoystickClipper.cpp>
#include "AHRS.h"

#include "MultiOutputPID.h"
#define THRESHHOLD_RANGE 2 //measured in degrees
#define DAVIDS_FUN_INPUT 20
#define FINAL_DEBOUNCE_TURN 300
#define STRAFEOVERRIDE_FACTOR .1

class MecanumDrive: public IControl {
protected:
	NewXboxController *xbox;
	RobotDrive* myRobot;
	Gyro *gyro;
	MultiOutputPID *motorOutput;
	MultiOutputPID *leftOutput;
	MultiOutputPID *rightOutput;
	Talon *talonFR, *talonRR, *talonFL, *talonRL;
	//FrontLeft3 RearLeft4 FrontRight1 RearRight2
	BeastSpeedControl *frontRight, *rearRight, *frontLeft, *rearLeft;
	PIDController *autoRotateController;
	PIDController *leftController;
	PIDController *rightController;

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
	Encoder *frontRightEncoder;
	Encoder *frontLeftEncoder;
	JoystickClipper *stickX;
	JoystickClipper *stickY;
	JoystickClipper *stickTwist;


	float x;
	float y;
	float twist;
	float angle;
	float speedFactor;
	float rotateSpeedFactor;
	float strafeSpeedFactor;
	bool creepMode;
	AHRS *imu;
	NetworkTable *table;
	SerialPort *serial_port;

public:
	MecanumDrive() {
		talonFR = new Talon(frontRightChannel);
		talonRR = new Talon(rearRightChannel);
		talonFL = new Talon(frontLeftChannel);
		talonRL = new Talon(rearLeftChannel);

		frontRight = new BeastSpeedControl(talonFR);
		rearRight = new BeastSpeedControl(talonRR);
		frontLeft = new BeastSpeedControl(talonFL);
		rearLeft = new BeastSpeedControl(talonRL);

		frontRightEncoder = new Encoder(0, 1, false);
		frontLeftEncoder = new Encoder(2, 3, false);

		stickX = new JoystickClipper();
		stickY = new JoystickClipper();
		stickTwist = new JoystickClipper();

		motorOutput = new MultiOutputPID(frontRight, frontLeft, rearRight,
				rearLeft, true);
		leftOutput = new MultiOutputPID(frontRight, NULL, rearRight, NULL,
				true);
		rightOutput = new MultiOutputPID(NULL, frontLeft, NULL, rearLeft, true);
		gyro = new Gyro(gyroChannel);

		autoRotateController = new PIDController(0.005, 0.0, 0.0, gyro,
				motorOutput);
		leftController = new PIDController(0.0001, 0.0, 0.0, frontLeftEncoder,
				leftOutput);
		rightController = new PIDController(0.0001, 0.0, 0.0, frontRightEncoder,
				rightOutput);
		//FrontLeft3 RearLeft4 FrontRight1 RearRight2
		myRobot = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight);
		myRobot->SetExpiration(0.1);
		xbox = NewXboxController::getInstance(1);
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

		table = NetworkTable::GetTable("datatable");
				serial_port = new SerialPort(57600, SerialPort::kMXP);
				uint8_t update_rate_hz = 50;
				imu = new AHRS(serial_port, update_rate_hz);
		creepMode = false;
	}

	void AutonomousInit() {
		lastCommand = 0;
		lastCommandTurn = 0;
		lastCommandDrive = 0;
		autoDriveCounter = 0;
		autoTurnCounter = 0;
		autoRotateController->Disable();
		leftController->Disable();
		rightController->Disable();
		gyro->Reset();
		motorOutput->DisableOverDrive();
		frontLeftEncoder->Reset();
		frontRightEncoder->Reset();
	}

	int AutonomousPeriodic(int input) {
		SmartDashboard::PutNumber("FrontRightEncoder",
				frontRightEncoder->Get());
		SmartDashboard::PutNumber("FrontLeftEncoder", frontLeftEncoder->Get());

		//if (lastCommand != input) {
		//	lastCommand = input;
		//}

		switch (input) {
		case 0:
			break;
		case 1:
			return turn(110);
		case 2:
			return turn(-110);
		case 3:
			return disableStuff();
		case 4:
			return drivePID(20000);
		default:
			if (abs(input) > 10) {
				return drive(abs(input / DAVIDS_FUN_INPUT), input > 0);
			}
			break;
		}
		return 0;
	}

	/*int driveForward() {
	 if (lastCommand == 0) {
	 autoDriveCounter = 5000 / DAVIDS_FUN_INPUT;
	 lastCommand = -1;
	 }
	 autoDriveCounter--;
	 if (autoDriveCounter <= 0) {
	 this->myRobot->ArcadeDrive(0.0, 0);
	 lastCommand = 0;
	 return 1;
	 }
	 this->myRobot->ArcadeDrive(0.5, 0);
	 return 0;
	 }*/

	int disableStuff() {
		AutonomousInit();
		return 0;
	}

	int drivePID(int input) {
		if (lastCommandDrive != input) {
			lastCommandDrive = input;
			leftController->SetSetpoint(input);
			rightController->SetSetpoint(input);
			leftController->Enable();
			rightController->Enable();
			autoTurnCounter = FINAL_DEBOUNCE_TURN / DAVIDS_FUN_INPUT;
		}
		double leftValue = frontLeftEncoder->Get();
		double rightValue = frontRightEncoder->Get();
		if (abs((leftValue - rightValue) / 2) < THRESHHOLD_RANGE) {
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
		AutonomousInit();
		myRobot->SetSafetyEnabled(false);
		SmartDashboard::PutBoolean("Use Gyro", false);
		SmartDashboard::PutNumber("Gyro Direction", gyro->GetAngle());
		SmartDashboard::PutBoolean("Creep Mode", false);

		autoRotateController->Disable();

		gyro->Reset();

		SmartDashboard::PutNumber("Override", 0.1);

	}

	void TeleopPeriodic() {
		float gyroAngle = gyro->GetAngle();
		SmartDashboard::PutNumber("Gyro Direction", gyroAngle);
/**
		if (fabs(x) < 0.1) {
		 x = 0.0;
		 }

		 if (fabs(y) < 0.1) {
		 y = 0.0;
		 }

		 if (fabs(twist) < 0.1) {
		 twist = 0.0;
		 }
*/

		 if (SmartDashboard::GetBoolean("Use Gyro")) {
		 angle = imu->GetYaw();
		 } else {
		 angle = 0.0;
		 }

		if (!xbox->isRBumperHeld()) {
			stickX->SetMaxValue(0.4);
			stickY->SetMaxValue(0.25);
			stickTwist->SetMaxValue(0.25);

		} else {
			stickX->SetMaxValue(1);
			stickY->SetMaxValue(1);
			stickTwist->SetMaxValue(1);
		}

		stickX->Update(xbox->getAxisLeftX(),xbox->getAxisLeftX());
		stickY->Update(xbox->getAxisLeftY(),xbox->getAxisLeftY());
		stickTwist->Update(xbox->getAxisRightX(),xbox->getAxisRightX());
		x = stickX->X();
		y = stickY->Y();
		twist = stickTwist->X();

		float override = x * SmartDashboard::GetNumber("Override");

		frontLeft->SetOverride(-override);
		frontRight->SetOverride(-override);

		myRobot->MecanumDrive_Cartesian(x, y, twist, 0);

		//myRobot->MecanumDrive_Cartesian(x, 0.0, 0.0, angle);

	}

	void setAutoProgram(int s) {
		//autoProgram = s;
	}

	void AutonomousExecute() {
		if ((lastCommand <= 0 && lastCommandTurn == 0 && lastCommandDrive <= 0)) {
			frontRight->PIDWrite(0.0);
			rearRight->PIDWrite(0.0);
			frontLeft->PIDWrite(0.0);
			rearLeft->PIDWrite(0.0);
		}
	}
};

