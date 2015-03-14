#include "WPILib.h"

#include <Gyro.h>
#include <IControl.h>
#include <RobotDrive.h>
#include <SmartDashboard/SmartDashboard.h>
#include <NewXboxController.h>
#include <cmath>
#include "WPILib.h"
#include <JoystickClipper.cpp>

#include "MultiOutputPID.h"
#define THRESHHOLD_RANGE 2 //measured in degrees
#define DAVIDS_FUN_INPUT 20
#define FINAL_DEBOUNCE_TURN 300

class DriveTest: public IControl {

protected:
	NewXboxController *xbox;
	RobotDrive* myRobot;
	Gyro *gyro;
	MultiOutputPID *motorOutput;
	MultiOutputPID *leftOutput;
	MultiOutputPID *rightOutput;
	Talon *motor1, *motor2, *motor3, *motor4;
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
		motor1 = new Talon(frontRightChannel);
		motor2 = new Talon(rearRightChannel);
		motor3 = new Talon(frontLeftChannel);
		motor4 = new Talon(rearLeftChannel);

		frontRightEncoder = new Encoder(0, 1, false);
		frontLeftEncoder = new Encoder(2, 3, false);

		motorOutput = new MultiOutputPID(motor1, motor3, motor2, motor4, true);
		leftOutput = new MultiOutputPID(motor1, NULL, motor2, NULL, true);
		rightOutput = new MultiOutputPID(NULL, motor3, NULL, motor4, true);

		gyro = new Gyro(gyroChannel);

		autoRotateController = new PIDController(0.005, 0.0, 0.0, gyro,
				motorOutput);
		leftController = new PIDController(0.0001, 0.0, 0.0, frontLeftEncoder,
				leftOutput);
		rightController = new PIDController(0.0001, 0.0, 0.0, frontRightEncoder,
				rightOutput);

		myRobot = new RobotDrive(motor3, motor4, motor1, motor2);
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
			stick->Update(xbox->getAxisLeftX(), xbox->getAxisLeftY());
			x = stick->X();
			y = stick->Y();
			SmartDashboard::PutNumber("Clipped X", x);
			SmartDashboard::PutNumber("Clipped Y", y);
			twist = xbox->getAxisRightX();
		} else {
			x = xbox->getAxisLeftX();
			y = xbox->getAxisLeftY();
			twist = xbox->getAxisRightX();
		}

		myRobot->MecanumDrive_Cartesian(x, y, twist, 0);
	}

};
