/*
 * JoystickClipper.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: Beast Laptop
 */

/*
 * LifterControll.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: Beasty
 */

#ifndef JOYSTICKCLIPPER_H
#define JOYSTICKCLIPPER_H

// Make look like arm class
#include "WPILib.h"

class JoystickClipper {

protected:
	float xValue;
	float yValue;
	float deadband;
	float maxLength;
	float minLength;

public:

	JoystickClipper() {
		deadband = 0.09;
		maxLength = 1.0;
		minLength = 0.15;
		xValue = 0.0;
		yValue = 0.0;
	}

	float X() {
		return xValue;
	}

	float Y() {
		return yValue;
	}

	float GetDeadband() {
		return deadband;
	}

	void SetDeadband(float value) {
		deadband = value;
	}

	float GetMaxValue() {
		return maxLength;
	}

	void SetMaxValue(float value) {
		maxLength = value;
	}

	float GetMinValue() {
		return minLength;
	}

	void SetMinValue(float value) {
		minLength = value;
	}

	void Update(float x, float y) {
		double vectorLength = sqrt(x * x + y * y);
		if (vectorLength <= deadband) {
			xValue = 0;
			yValue = 0;
		} else {
			xValue = ClipAxis(x);
			yValue = ClipAxis(y);
		}
	}

private:
	float ClipAxis(float value){
		if (fabs(value)<=deadband){
			return 0.0;
		}
		float percent = (fabs(value)-deadband)/(1-deadband);
		if (value>0){
			return minLength + percent*(maxLength-minLength);
		}
		return -minLength - percent*(maxLength-minLength);
	}
};

#endif

