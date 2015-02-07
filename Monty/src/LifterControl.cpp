/*
 * LifterControll.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: Beasty
 */

// Make look like arm class
#include "WPILib.h"
#include "XboxController.h"
#include "Math.h"
#include "IControl.h"

#define TOLERANCE 25
#define HOME 1
#define LEVEL_1 500
#define LEVEL_2 1000
#define LEVEL_3 1500
#define MOTOR_SPEED -1.0
#define MOTOR_SPEED_DOWN 1.0
#define HOLD_SPEED 0.05

class LifterControl : public IControl {

	Encoder *en1;
	Talon *lifter;
	DigitalInput *upperLimit;
	DigitalInput *lowerLimit;
	XboxController *xbox;
	Timer *time;

	double speedFactor;

public:

	int level1Value;
	int level2Value;
	int level3Value;
	int homeValue;
	double lifterSpeed;
	double accelerationSpeedUp;
	double accelerationSpeedDown;

	LifterControl() {
		en1 = new Encoder(0, 1);
		en1->Reset();
		lifter = new Talon(6);
		level1Value = LEVEL_1;
		level2Value = LEVEL_2;
		level3Value = LEVEL_3;
		homeValue = HOME;
		upperLimit = new DigitalInput(3);
		lowerLimit = new DigitalInput(4);
		lifterSpeed = 0;
		speedFactor = 1.0;
		xbox = XboxController::getInstance();
		time = new Timer();
		accelerationSpeedUp = 0.05;
		accelerationSpeedDown = 0.05;
	}


	void lifterupdate() {
		bool movingUp = lifterSpeed < 0;
		bool movingDown = lifterSpeed > 0;
		bool canGoUp = (upperLimit->Get() && movingUp);
		bool canGoDown = (lowerLimit->Get() && movingDown);
		if (canGoUp && lifterSpeed <= -0.1) {
			lifter->Set(speedFactor * lifterSpeed);
		} else if (canGoDown && lifterSpeed >= 0.1) {
			lifter->Set(speedFactor * lifterSpeed);
		} else {
			lifter->Set(0.0);
		}
	}

	void SetSpeepFactor(double factor) {
		speedFactor = factor;
	}

	void ManualMode() {

		bool isUpperLimit = GetUpperLimit();
		bool isLowerLimit = GetLowerLimit();
		double yAxis = xbox->getAxisRightY();

		if (isUpperLimit && yAxis > 0) {
			lifterSpeed = -yAxis;
		} else if (isLowerLimit && yAxis < 0) {
			lifterSpeed = -yAxis;
		} else {
			lifterSpeed = 0;
		}
	}

	void SetEncoderValue() {
		if (!lowerLimit->Get()) {
			lifterSpeed = MOTOR_SPEED_DOWN;
		} else {
			lifterSpeed = 0;
			en1->Reset();
		}
	}

	void Stop() {
		float speed=accelerationSpeedUp>accelerationSpeedDown?accelerationSpeedUp:accelerationSpeedDown;

		if ((lifterSpeed<speed&&lifterSpeed>0)||(lifterSpeed>speed&&lifterSpeed<0)) lifterSpeed=0;

		if (lifterSpeed > 0) {
			lifterSpeed -= accelerationSpeedUp;
		} else if (lifterSpeed < 0) {
			lifterSpeed += accelerationSpeedDown;
		}
	}

	void SetAccelUp(double up) {
		accelerationSpeedUp = up;
	}

	void SetAccelDown(double down) {
		accelerationSpeedDown = down;
	}
	//TODO
	void MoveUp() {
		if (lifterSpeed > 0) {
			lifterSpeed = 0;
		}
		if (upperLimit->Get()) {
			if (lifterSpeed > MOTOR_SPEED) {
				lifterSpeed -= accelerationSpeedUp;
			} else {
				lifterSpeed = MOTOR_SPEED;
			}
		} else {
			lifterSpeed = 0;
		}
	}

	void MoveDown() {
		if (lifterSpeed < 0) {
			lifterSpeed = 0;
		}
		if (lowerLimit->Get()) {
			if (lifterSpeed < MOTOR_SPEED_DOWN) {
				lifterSpeed += accelerationSpeedDown;
			} else {
				lifterSpeed = MOTOR_SPEED_DOWN;
			}
		} else {
			lifterSpeed = 0;
		}
	}




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
	}

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

	int GetEnconder() {
		return en1->Get();
	}

	bool GetUpperLimit() {
		return upperLimit->Get();
	}

	bool GetLowerLimit() {
		return lowerLimit->Get();
	}

private:

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
	}

};
