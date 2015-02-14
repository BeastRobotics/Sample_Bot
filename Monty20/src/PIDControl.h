/*
 * PIDLifterControl.h
 *
 *  Created on: Jan 31, 2015
 *      Author: education
 */

#include "WPILib.h"

#ifndef SRC_PIDCONTROL_H_
#define SRC_PIDCONTROL_H_

class PIDControl {
public:
	PIDControl(Encoder *nSource, PIDOutput *nOutput);
	virtual ~PIDControl();

	bool start(float np=0.05, float ni=0, float nd=0);

	bool disable();

	void setPoint(float x=0);

	bool brake(float np, float ni, float nd);

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
