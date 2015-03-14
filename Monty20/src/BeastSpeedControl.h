/*
 * BeastSpeedControl.h
 *
 *  Created on: Mar 14, 2015
 *      Author: Beast Laptop
 */

#ifndef SRC_BEASTSPEEDCONTROL_H_
#define SRC_BEASTSPEEDCONTROL_H_
#include "WPILib.h"

class BeastSpeedControl:public SpeedController {
public:
	BeastSpeedControl(SpeedController* speedControl);
	~BeastSpeedControl();
	void Set(float speed, uint8_t syncGroup=0);
	float Get();
	void Disable();
	void SetOverride(float override);
	void PIDWrite(float output);
private:
	SpeedController* controller;
	float override;
};

#endif /* SRC_BEASTSPEEDCONTROL_H_ */
