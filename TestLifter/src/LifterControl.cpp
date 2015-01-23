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


	Encoder *en1 = new Encoder(0, 1, true, Encoder::k2X);
	Talon *lifter;

public:


	LifterControl() {
		en1->Reset();
	}

	void MoveToLevel1() {
		if (en1->Get() < LEVEL_1 && en1->Get() > LEVEL_1 + TOLERANCE) {
			lifter->Set(MOTOR_SPEED);
			MoveToLevel1();
		} else if (en1->Get() > LEVEL_1 && en1->Get() < LEVEL_1 - TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
			MoveToLevel1();
		} else {
			lifter->Set(0);
		}
	}

	void MoveToLevel2() {
		if (en1->Get() < LEVEL_2 && en1->Get() > LEVEL_2 + TOLERANCE) {
			lifter->Set(MOTOR_SPEED);
			MoveToLevel2();
		} else if (en1->Get() > LEVEL_2 && en1->Get() < LEVEL_2 - TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
			MoveToLevel2();
		} else {
			lifter->Set(0);
		}
	}

	void MoveToLevel3() {
		if (en1->Get() < LEVEL_3 && en1->Get() > LEVEL_3 + TOLERANCE) {
			lifter->Set(MOTOR_SPEED);
			MoveToLevel3();
		} else if (en1->Get() > LEVEL_3 && en1->Get() < LEVEL_3 - TOLERANCE) {
			lifter->Set(-MOTOR_SPEED);
			MoveToLevel3();
		} else {
			lifter->Set(0);
		}
	}

};
