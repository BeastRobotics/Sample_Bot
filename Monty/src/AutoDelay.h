/*
 * AutoDelay.h
 *
 *  Created on: Feb 14, 2015
 *      Author: Beast Laptop
 */

#ifndef SRC_AUTODELAY_H_
#define SRC_AUTODELAY_H_
#include "IControl.h"

class AutoDelay:public IControl {
	int autoCount;
	int prevCommand;
public:
	AutoDelay();
	virtual ~AutoDelay();
	int AutonomousPeriodic(int input);
	void AutonomousInit();
};

#endif /* SRC_AUTODELAY_H_ */
