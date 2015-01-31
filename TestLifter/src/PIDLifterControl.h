/*
 * PIDLifterControl.h
 *
 *  Created on: Jan 31, 2015
 *      Author: education
 */

#include "WPILib.h"

#ifndef SRC_PIDLIFTERCONTROL_H_
#define SRC_PIDLIFTERCONTROL_H_

class PIDLifterControl {
public:
	PIDLifterControl(Encoder *nSource, PIDOutput *nOutput);
	virtual ~PIDLifterControl();

	bool start(float np, float ni, float nd);

	bool disable();

	void setPoint(float x);

	bool brake();

	bool release();


private:
	PIDController *controller;
	Encoder *source;
	PIDOutput *output;
	float p;
	float i;
	float d;

};

#endif /* SRC_PIDLIFTERCONTROL_H_ */
