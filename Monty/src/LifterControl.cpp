/*
 * LifterControll.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: Beasty
 */

#ifndef LIFTERCONTROL_H
#define LIFTERCONTROL_H

// Make look like arm class
#include "WPILib.h"
#include "XboxController.h"
#include "Math.h"
#include "IControl.h"
#include <cTime>

#define TOLERANCE 25
#define HOME 1
#define LEVEL_1 500
#define LEVEL_2 1000
#define LEVEL_3 1500
#define MOTOR_SPEED -1.0
#define MOTOR_SPEED_DOWN 1.0
#define HOLD_SPEED 0.05

#define LIFTER_MOTOR 5
#define LOWER_LIMIT 5
#define UPPER_LIMIT 4

class LifterControl: public IControl {

protected:
	Talon *lifter;
	DigitalInput *upperLimit;
	DigitalInput *lowerLimit;
	//DigitalInput *magInput;
	XboxController *xbox;
	Timer *time;
	double speedFactor;

public:

	int level1Value;
	int level2Value;
	int level3Value;
	int homeValue;
	double lifterSpeed;
	double acceleration;
	int autoProgram = 0;
	float UpperLimitValue;
	float LowerLimitValue;

	LifterControl() {
		lifter = new Talon(LIFTER_MOTOR);
		level1Value = LEVEL_1;
		level2Value = LEVEL_2;
		level3Value = LEVEL_3;
		homeValue = HOME;
		upperLimit = new DigitalInput(UPPER_LIMIT);
		lowerLimit = new DigitalInput(LOWER_LIMIT);
		//magInput = new DigitalInput(5);
		lifterSpeed = 0;
		speedFactor = 1.0;
		xbox = XboxController::getInstance();
		time = new Timer();
		acceleration = 0.05;

		UpperLimitValue = 0.0;
		LowerLimitValue = 0.0;
	}

	void RobotInit() {
	}
	void DisabledInit() {
	}
	void AutonomousInit(int autoProg) {
		setAutoProgram(autoProg);
	}

	int AutonomousPeriodic() {
		/*switch (autoProgram) {
		case 1:
			//Drive to auto
			break;
		case 2:
			//Take RC to auto

			break;
		case 3:
			//Take tote to auto
			break;
		case 4:
			//Take RC and tote to Auto
			break;
		default:
			//Drive to auto
			break;
		}*/
		return 0;
	}
	void TeleopInit() {
		SmartDashboard::PutNumber("Lifter Encoder", 0.0);
		SmartDashboard::PutBoolean("Manual Lifter Mode", true);
		SmartDashboard::PutNumber("Lifter Speed Factor", 1.0);
		SmartDashboard::PutNumber("Lifter Motor Value", 0.0); //This is the current output to the motor
		SmartDashboard::PutNumber("Accel", 0.1); //Acceleration going up
		SmartDashboard::PutBoolean("Mag Input", false);
		//SetEncoderValue();
		//Stop();
		lifter->Set(0.0);
	}
	void TeleopPeriodic() {
		SmartDashboard::PutBoolean("Lower Limit", GetUpperLimit());
		SmartDashboard::PutBoolean("Upper Limit", GetLowerLimit());
		//SmartDashboard::PutBoolean("Mag Input", GetMagInput());
		SmartDashboard::PutNumber("Lifter Motor Value", lifterSpeed);
		SetSpeepFactor(SmartDashboard::GetNumber("Lifter Speed Factor"));
		SetAccel(SmartDashboard::GetNumber("Accel"));

		bool isLifterManual = SmartDashboard::GetBoolean("Manual Lifter Mode");

		if (isLifterManual) {

			if (xbox->isBHeld()) {
				MoveUp();
			} else if (xbox->isXHeld()) {
				MoveDown();
			} else {
				Stop();
			}

		} else {
			Stop();
			/*
			if (xbox->isLeftTriggerHeld()) {
				if (xbox->isBHeld()) {
					MoveToLevel1();
				} else if (xbox->isYHeld()) {
					MoveToLevel2();
				} else if (xbox->isXHeld()) {
					MoveToLevel3();
				} else if (xbox->isAHeld()) {
					MoveToHome();
				} else {
					Stop();
				}
			} */
		} //End big if and lifter move if

		lifterupdate();
		//lifter->Set(0.0);
	}
	void lifterupdate() {
		UpperLimitValue = GetUpperLimit();
		LowerLimitValue = GetLowerLimit();
		double finalSpeed=0.0;
		bool movingUp = lifterSpeed < 0;
		bool movingDown = lifterSpeed > 0;
		bool canGoUp = (UpperLimitValue && movingUp);
		bool canGoDown = (LowerLimitValue && movingDown);

		if (canGoUp && lifterSpeed <= -0.1) {
			finalSpeed=speedFactor * lifterSpeed;
		} else if (canGoDown && lifterSpeed >= 0.1) {
			finalSpeed=speedFactor * lifterSpeed;
		} else {
			finalSpeed=0.0;
		}

		if (finalSpeed==0.0) {
			this->brake();
		} else {
			this->release();
			lifter->Set(finalSpeed);
		}
	}

	void SetSpeepFactor(double factor) {
		speedFactor = factor;
	}

	void setAutoProgram(int s) {
		autoProgram = s;
	}

	/*void SetEncoderValue() {
		if (!lowerLimit->Get()) {
			lifterSpeed = MOTOR_SPEED_DOWN;
		} else {
			lifterSpeed = 0;
			en1->Reset();
		}
	}*/

	/*
	 * Gets a velocity based on the function f(x) = x^3.
	 * It gets the lifterSpeed's cube root which is the x value,
	 * add/subtracts the parameter rate, then cubes that value and returns it.
	 * @param rate the rate of change of velocity or delta x.
	 */
	double getVelocity(double desiredVelocity, double rate) {
		if (std::abs(desiredVelocity - lifterSpeed) <= 0.01)
			return lifterSpeed;
		double x = std::cbrt(lifterSpeed * 4);
		if (desiredVelocity > lifterSpeed) {
			x += rate;
		} else {
			x -= rate;
		}
		return ((int) ((x * x * x * (1.0 / 4.0)) * 10E8)) / 10E8; // removes insignificant decimal values before returning
	}

	//TODO
	void Stop() {
		float speed = acceleration;

		if ((lifterSpeed < speed && lifterSpeed > 0)
				|| (lifterSpeed > speed && lifterSpeed < 0))
			lifterSpeed = 0;

		lifterSpeed = getVelocity(0, acceleration);
	}

	void SetAccel(double n) {
		acceleration = n;
	}

	//TODO
	void MoveUp() {
		lifterSpeed = getVelocity(MOTOR_SPEED, acceleration);
		if (!GetUpperLimit()) {
			lifterSpeed = 0;
		}
	}

	void MoveDown() {
		lifterSpeed = getVelocity(MOTOR_SPEED_DOWN, acceleration);
		if (!GetLowerLimit()) {
			lifterSpeed = 0;
		}
	}

	/*
	void MoveToHome() {

		if (lowerLimit->Get()) {
			MoveToHomePrivate();
		} else {
			lifterSpeed = 0;
		}
	}

	void MoveToLevel1() {
		if (upperLimit->Get()) {
			MoveToLevel1Private();
		} else {
			lifterSpeed = 0;
		}
	}

	void MoveToLevel2() {
		if (upperLimit->Get()) {
			MoveToLevel2Private();
		} else {
			lifterSpeed = 0;
		}
	}

	void MoveToLevel3() {
		if (upperLimit->Get()) {
			MoveToLevel3Private();
		} else {
			lifterSpeed = 0;
		}
	} */
	void SetHome(int homeValue) {
		this->homeValue = homeValue;
	}

	void SetLevel1(int level1Value) {
		this->level1Value = level1Value;
	}

	void SetLevel2(int level1Value) {
		this->level2Value = level2Value;
	}

	void SetLevel3(int level1Value) {
		this->level3Value = level3Value;
	}


	bool GetUpperLimit() {
		return lowerLimit->Get();  //Backwords
	}

	bool GetLowerLimit() {
		return upperLimit->Get();  //backwords
	}

	/*
	bool GetMagInput() {
		return magInput->Get();
	} */

	virtual void brake() {
		lifter->Set(0.0);
	}

	virtual void release() {

	}

private:

	/*
	void MoveToHomePrivate() {
		if (en1->Get() > HOME + TOLERANCE) {
			lifterSpeed = MOTOR_SPEED_DOWN;
		} else {
			en1->Reset();
			lifterSpeed = 0;
		}
	}

	void MoveToLevel1Private() {
		if (en1->Get() < LEVEL_1 - TOLERANCE) {
			lifterSpeed = MOTOR_SPEED;
		} else if (en1->Get() > LEVEL_1 + TOLERANCE) {
			lifterSpeed = -MOTOR_SPEED;
		} else {
			lifterSpeed = 0;
		}
	}

	void MoveToLevel2Private() {
		if (en1->Get() < LEVEL_2 - TOLERANCE) {
			lifterSpeed = MOTOR_SPEED;
		} else if (en1->Get() > LEVEL_2 + TOLERANCE) {
			lifterSpeed = -MOTOR_SPEED;
		} else {
			lifterSpeed = 0;
		}
	}

	void MoveToLevel3Private() {
		if (en1->Get() < LEVEL_3 - TOLERANCE) {
			lifterSpeed = MOTOR_SPEED;
		} else if (en1->Get() > LEVEL_3 + TOLERANCE) {
			lifterSpeed = -MOTOR_SPEED;
		} else {
			lifterSpeed = 0;
		}
	} */
};

#endif
