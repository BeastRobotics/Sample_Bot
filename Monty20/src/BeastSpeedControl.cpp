/*
 * BeastSpeedControl.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Beast Laptop
 */

#include <BeastSpeedControl.h>

BeastSpeedControl::BeastSpeedControl(SpeedController* speedControl) {
	controller = speedControl;
	override = 0.0;
}

BeastSpeedControl::~BeastSpeedControl() {
}
void BeastSpeedControl::Set(float speed, uint8_t syncGroup) {
	float newSpeed = speed + override;
	if (newSpeed > 1) {
		newSpeed = 1.0;
	}
	if (newSpeed < -1) {
		newSpeed = -1.0;
	}
	this->controller->Set(newSpeed, syncGroup);
}
void BeastSpeedControl::SetOverride(float override) {
	this->override = override;
}

