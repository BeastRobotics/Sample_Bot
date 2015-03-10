#include "WPILib.h"
#include "XboxController.h"
/**
 * This is a demo program showing how to use Mecanum control with the RobotDrive class.
 */
class Robot: public SampleRobot {

	// Channels for the wheels
	const static int frontLeftChannel = 2;
	const static int rearLeftChannel = 4;
	const static int frontRightChannel = 1;
	const static int rearRightChannel = 3;

	const static int joystickChannel = 0;

	RobotDrive robotDrive;	// robot drive system
	XboxController *xbox;		// only joystick
	Talon *motor1;
	Talon *motor2;
	Talon *motor3;
	Talon *motor4;
	RobotDrive *myRobot;

public:
	Robot(){
		motor1 = new Talon(frontRightChannel);
		motor2 = new Talon(rearRightChannel);
		motor3 = new Talon(frontLeftChannel);
		motor4 = new Talon(rearLeftChannel);
		myRobot = new RobotDrive(motor3, motor4, motor1, motor2);
		myRobot->SetExpiration(0.1);
		xbox = XboxController::getInstance();
		myRobot->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true); // invert the left side motors
		myRobot->SetInvertedMotor(RobotDrive::kRearLeftMotor, true); // you may need to change or remove this to match your robot
		robotDrive.SetExpiration(0.1);
		robotDrive.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		robotDrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);// you may need to change or remove this to match your robot
	}

	/**
	 * Runs the motors with Mecanum drive.
	 */
	void OperatorControl() {
		robotDrive.SetSafetyEnabled(false);
		while (IsOperatorControl() && IsEnabled()) {
			// Use the joystick X axis for lateral movement, Y axis for forward movement, and Z axis for rotation.
			// This sample does not use field-oriented drive, so the gyro input is set to zero.
			robotDrive.MecanumDrive_Cartesian(xbox->getAxisLeftX(),xbox->getAxisLeftY(),xbox->getAxisRightX());

			Wait(0.005); // wait 5ms to avoid hogging CPU cycles
		}
	}

};

START_ROBOT_CLASS(Robot);
