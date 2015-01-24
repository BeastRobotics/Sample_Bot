/*
 * LifterControll.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: Beasty
 */

#include "WPILib.h"
#include "XboxController.h"

#define TOLERANCE 25
#define HOME 1
#define LEVEL_1 300
#define LEVEL_2 600
#define LEVEL_3 900
#define MOTOR_SPEED 0.1

class LifterControl {

	Encoder *en1;
	Talon *lifter;
	DigitalInput *upperLimit;
	DigitalInput *lowerLimit;

public:

	LifterControl() {
		en1 = new Encoder(0, 1, true, Encoder::k2X);
		lifter = new Talon(6);
		en1->Reset();
		level1Value = LEVEL_1;
		level2Value = LEVEL_2;
		level3Value = LEVEL_3;
		homeValue = HOME;
		upperLimit = new DigitalInput(3);
		lowerLimit = new DigitalInput(4);
	}

	void Stop() {
		lifter->Set(0);
	}

	void MoveToHome() {
		if (lowerLimit->Get()) {
			MoveToHomePrivate();
		}
	}

	void MoveToLevel1() {
		if (upperLimit->Get()) {
			MoveToLevel1Private();
		}
	}

	void MoveToLevel2() {
		if (upperLimit->Get()) {
			MoveToLevel2Private();
		}
	}

	void MoveToLevel3() {
		if (upperLimit->Get()) {
			MoveToLevel3Private();
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

private:
	int level1Value;
	int level2Value;
	int level3Value;
	int homeValue;

	void MoveToHomePrivate() {
		if (en1->Get() > HOME + TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
		} else {
			lifter->Set(0);
		}
	}

	void MoveToLevel1Private() {
		if (en1->Get() < LEVEL_1 - TOLERANCE) {
			lifter->Set(MOTOR_SPEED);
		} else if (en1->Get() > LEVEL_1 + TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
		} else {
			lifter->Set(0);
		}
	}

	void MoveToLevel2Private() {
		if (en1->Get() < LEVEL_2 - TOLERANCE) {
			lifter->Set(MOTOR_SPEED);
		} else if (en1->Get() > LEVEL_2 + TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
		} else {
			lifter->Set(0);
		}
	}

	void MoveToLevel3Private() {
		if (en1->Get() < LEVEL_3 - TOLERANCE) {
			lifter->Set(MOTOR_SPEED);
		} else if (en1->Get() > LEVEL_3 + TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
		} else {
			lifter->Set(0);
		}
	}

};
