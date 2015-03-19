#include "WPILib.h"

#include <Gyro.h>
#include <IControl.h>
#include <RobotDrive.h>
#include <SmartDashboard/SmartDashboard.h>
#include <NewXboxController.h>
#include <cmath>
#include "WPILib.h"
#include <JoystickClipper.cpp>

#include "BeastSpeedControl.h"

#include "MultiOutputPID.h"
#define THRESHHOLD_RANGE 2 //measured in degrees
#define DAVIDS_FUN_INPUT 20
#define FINAL_DEBOUNCE_TURN 300
#define STRAFEOVERRIDE_FACTOR 0

class DriveTest: public IControl {

protected:
	NewXboxController *xbox;
	RobotDrive* myRobot;
	Gyro *gyro;
	MultiOutputPID *motorOutput;
	MultiOutputPID *leftOutput;
	MultiOutputPID *rightOutput;
	//FrontLeft3 RearLeft4 FrontRight1 RearRight2
	Talon *talonFR, *talonRR, *talonFL, *talonRL;
	BeastSpeedControl *frontRight, *rearRight, *frontLeft, *rearLeft;
	PIDController *autoRotateController;
	PIDController *leftController;
	PIDController *rightController;
	JoystickClipper *stick;

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

	float x;
	float y;
	float twist;
	float angle;
	float speedFactor;
	float rotateSpeedFactor;
	float strafeSpeedFactor;
	bool creepMode;

public:
	DriveTest() {
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

		myRobot = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight);
		myRobot->SetExpiration(0.1);

		xbox = NewXboxController::getInstance(1);
		stick = new JoystickClipper();

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

	void creepModeSet() {
		if (xbox->isRightTriggerHeld()) {
			creepMode = true;
		}
	}

	void TeleopInit() {
		gyro->Reset();
		AutonomousInit();
		myRobot->SetSafetyEnabled(false);
		SmartDashboard::PutBoolean("Use Gyro", false);
		SmartDashboard::PutNumber("Gyro Direction", gyro->GetAngle());
		SmartDashboard::PutNumber("Y Speed Factor", 1.0);
		SmartDashboard::PutNumber("Rotate Speed Factor", 0.5);
		SmartDashboard::PutNumber("Strafe Speed Factor", 0.5);
		SmartDashboard::PutNumber("Test Value", 0);
		SmartDashboard::PutNumber("Drive Value", 0.0);
		SmartDashboard::PutNumber("Clipped X", 0);
		SmartDashboard::PutNumber("Clipped Y", 0);
		SmartDashboard::PutNumber("Override", 0);

		stick->SetMaxValue(0.5);
	}

	void TeleopPeriodic() {
		y = 0;

		x = twist = y;
		x = 0;
		//float ySet = SmartDashboard::GetNumber("Test Value");
		SmartDashboard::PutNumber("Xbox Y", xbox->getAxisLeftY());
		SmartDashboard::PutNumber("Xbox X", xbox->getAxisLeftX());

		if (xbox->isBHeld()) {
			y = SmartDashboard::GetNumber("Drive Value");
		} else if (xbox->isAHeld()) {
			stick->SetMaxValue(0.4);
		} else {
			stick->SetMaxValue(1);
		}

		stick->Update(xbox->getAxisLeftX(), xbox->getAxisLeftY());
		x = stick->X();
		y = stick->Y();
		twist = xbox->getAxisRightX();
		SmartDashboard::PutNumber("Clipped X", x);
		SmartDashboard::PutNumber("Clipped Y", y);

		float override = x * SmartDashboard::GetNumber("Override");
		frontLeft->SetOverride(-override);
		frontRight->SetOverride(-override);

		myRobot->MecanumDrive_Cartesian(x, y, twist);
		/*		x = .5;
		 float xSet = SmartDashboard::GetNumber("Strafe Speed Factor");
		 if (xbox->isAHeld())
		 ExDriveRight(x);
		 else if (xbox->isBHeld())
		 ExDrivLefte(x);
		 else
		 rDis();
		 */
		//myRobot->MecanumDrive_Cartesian(x, y, twist, 0);
	}
	void ExDrivLefte(float x) {
		frontRight->Set(x);
		rearRight->Set(-x);
		frontLeft->Set(x);
		rearLeft->Set(-x);
	}
	void ExDriveRight(float x) {
		frontRight->Set(-x);
		rearRight->Set(x);
		frontLeft->Set(-x);
		rearLeft->Set(x);
	}
	void rDis() {
		frontRight->Set(0);
		rearRight->Set(0);
		frontLeft->Set(0);
		rearLeft->Set(0);
	}

};
