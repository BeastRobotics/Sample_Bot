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
	virtual ~BeastSpeedControl();
	void Set(float speed, uint8_t=0);
	void SetOverride(float override);
private:
	SpeedController* controller;
	float override;
};

#endif /* SRC_BEASTSPEEDCONTROL_H_ */
