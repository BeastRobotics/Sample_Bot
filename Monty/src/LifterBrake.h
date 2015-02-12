/*
 * LifterBrake.h
 *
 *  Created on: Feb 11, 2015
 *      Author: Beasty
 */

#ifndef SRC_LIFTERBRAKE_H_
#define SRC_LIFTERBRAKE_H_

#include "WPILib.h"
#include "NewXboxController.h"
#include "LifterControl.cpp"
#include "PIDControl.h"

class LifterBrake: public LifterControl {
private:
	PIDControl *pidControl;
	Preferences *pref;
	double p, i, d;
	Encoder *en1;

public:
	LifterBrake();
	virtual ~LifterBrake();

	void brake();
	void release();
	void TeleopPeriodic();

	int GetEncoder();
};

#endif /* SRC_LIFTERBRAKE_H_ */
