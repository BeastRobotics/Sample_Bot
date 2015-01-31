/*
 * LifterControll.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: Beasty
 */


// Make look like arm class
#include "WPILib.h"
#include "XboxController.h"

#define TOLERANCE 25
#define HOME 1
#define LEVEL_1 500
#define LEVEL_2 1000
#define LEVEL_3 1500
#define MOTOR_SPEED -0.50
#define MOTOR_SPEED_DOWN 0.50
#define HOLD_SPEED 0.05

class LifterControl {

	Encoder *en1;
	Talon *lifter;
	DigitalInput *upperLimit;
	DigitalInput *lowerLimit;
	Joystick *stick;
	double lifterSpeed;
	double speedFactor;


public:

	int level1Value;
	int level2Value;
	int level3Value;
	int homeValue;

	LifterControl(Joystick *xStick) {
		en1 = new Encoder(0, 1);
		en1->Reset();
		lifter = new Talon(4);
		level1Value = LEVEL_1;
		level2Value = LEVEL_2;
		level3Value = LEVEL_3;
		homeValue = HOME;
		upperLimit = new DigitalInput(3);
		lowerLimit = new DigitalInput(4);
		stick = xStick;
		lifterSpeed = 0;
		speedFactor = 1.0;
	}

	void lifterupdate () {
		lifter->Set(speedFactor * lifterSpeed);
	}

	void SetSpeepFactor(double factor) {
		speedFactor = factor;
	}

	void HoldPosition() {
		speedFactor = HOLD_SPEED;
	}

	void ManualMode() {
		if (upperLimit->Get() && stick->GetY() > 0) {
			lifterSpeed = stick->GetY();
		} else if (lowerLimit->Get() && stick->GetY() < 0) {
			lifterSpeed = stick->GetY();
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
		lifterSpeed = 0;
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
