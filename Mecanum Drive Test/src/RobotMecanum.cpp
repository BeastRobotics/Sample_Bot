#include "WPILib.h"
#include "XboxController.h"
#include "Math.h"

/**
 * This is a demo program showing how to use Mecanum control with the RobotDrive class.
 */
class RobotMecanum: public SampleRobot {

	// Channels for the wheels
	const static int frontLeftChannel = 2;
	const static int rearLeftChannel = 4;
	const static int frontRightChannel = 1;
	const static int rearRightChannel = 3;

	const static int joystickChannel = 0;

	RobotDrive robotDrive;	// robot drive system
	Joystick stick;			// only joystick
	XboxController *xbox;
	Gyro *gyro;
	float strafe;
	float strafeFactor;
	float x;
	float y;
	float twist;
	float angle;
	float speedFactor;

public:
	RobotMecanum() :
			robotDrive(frontLeftChannel, rearLeftChannel, frontRightChannel,
					rearRightChannel),// these must be initialized in the same order
			stick(joystickChannel)				// as they are declared above.
	{
		robotDrive.SetExpiration(0.1);
		robotDrive.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		robotDrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);// you may need to change or remove this to match your robot
		xbox = xbox->getInstance();
		strafe = 0.0;
		strafeFactor = 1.0;
		x = 0.0;
		y = 0.0;
		twist = 0.0;
		angle = 0.0;
		speedFactor = 0.5;
		gyro = new Gyro(1);
	}

	/**
	 * Runs the motors with Mecanum drive.
	 */
	void OperatorControl() {
		robotDrive.SetSafetyEnabled(false);
		SmartDashboard::PutBoolean("Use Gyro", false);

		SmartDashboard::PutNumber("Speed Factor", speedFactor);

		gyro->Reset();

		while (IsOperatorControl() && IsEnabled()) {
			//strafeFactor = abs(xbox->getAxisTrigger());

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

			SmartDashboard::PutNumber("X Value", x);
			SmartDashboard::PutNumber("Y Value", y);
			SmartDashboard::PutNumber("Twist Value", twist);
			SmartDashboard::PutNumber("Gyro Angle", gyro->GetAngle());

			speedFactor = SmartDashboard::GetNumber("Speed Factor");

			x *= speedFactor;
			y *= speedFactor;
			twist *= speedFactor;
			/*
			 xbox->getAxisTrigger();
			 if (xbox->isRBumperHeld()) {
			 strafe = strafeFactor * 1.0;
			 } else if (xbox->isLBumperHeld()) {
			 strafe = strafeFactor * -1.0;
			 } else {
			 strafe = 0.0;
			 }*/

			// Use the joystick X axis for lateral movement, Y axis for forward movement, and Z axis for rotation.
			// This sample does not use field-oriented drive, so the gyro input is set to zero.
			robotDrive.MecanumDrive_Cartesian(x, y, twist, angle);
			//robotDrive.TankDrive(xbox->getLeftStick(),xbox->getRightStick());

			//robotDrive.MecanumDrive_Cartesian(xbox->getAxisLeftX(), xbox->getAxisLeftY(), strafe);

			Wait(0.005); // wait 5ms to avoid hogging CPU cycles
		}
	}

};

START_ROBOT_CLASS(RobotMecanum);
