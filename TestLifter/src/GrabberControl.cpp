#include "WPILib.h"
#include "XboxController.h"

#define ARM_LEFT 5
#define ARM_RIGHT 6
#define ARM_MOTOR_SPEED 0.1
#define ARM_MOTOR_SPEED_BACK -0.10

class GrabberControl {

	Talon *armLeft;
	Talon *armRight;
	Joystick *stick;
	DoubleSolenoid *sol1;


public:

	double leftSpeed;
	double rightSpeed;
	double speedFactor;

	GrabberControl(Joystick *xStick) {
		//armLeft = new Talon(ARM_LEFT);
		//armRight = new Talon(ARM_RIGHT);
		stick = xStick;
		sol1 = new DoubleSolenoid(0, 1);
		leftSpeed = 0;
		rightSpeed = 0;
		speedFactor = 1;
		sol1->Set(sol1->kOff);
	}

	void DriveWithStick() {
		leftSpeed = stick->GetY();
		rightSpeed = stick->GetY();
	}

	void Stop() {
		leftSpeed = 0;
		rightSpeed = 0;
	}

	void DriveIn() {
		leftSpeed = ARM_MOTOR_SPEED;
		rightSpeed = ARM_MOTOR_SPEED;
	}

	void DriveOut() {
		leftSpeed = ARM_MOTOR_SPEED_BACK;
		rightSpeed = ARM_MOTOR_SPEED_BACK;
	}

	void SpinClockWise() {
		leftSpeed = ARM_MOTOR_SPEED;
		rightSpeed = ARM_MOTOR_SPEED_BACK;
	}

	void SpinCounterClockWise() {
		leftSpeed = ARM_MOTOR_SPEED_BACK;
		rightSpeed = ARM_MOTOR_SPEED;
	}

	void SetGrabberArm(bool isExtended) {
		if (isExtended) {
			sol1->Set(sol1->kForward);
		} else {
			sol1->Set(sol1->kReverse);
		}
	}

	void SetSpeeds(double factor) {
		speedFactor = factor;
	}

	void SetMotors() {
		armLeft->Set(speedFactor * leftSpeed);
		armRight->Set(speedFactor * (-rightSpeed));
	}
};
