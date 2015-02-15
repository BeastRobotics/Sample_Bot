/*
 * MultiOutputPID.h
 *
 *  Created on: Feb 13, 2015
 *      Author: David
 */

#ifndef MULTIOUTPUTPID_H_
#define MULTIOUTPUTPID_H_

#include "WPILib.h"
#include <PIDOutput.h>

class MultiOutputPID:public PIDOutput {

	PIDOutput *motor1, *motor2, *motor3, *motor4;
	bool isRotate;
	bool overDriveOn;
	float overDrive;

public:
	MultiOutputPID(PIDOutput *motor1, PIDOutput *motor2, PIDOutput *motor3, PIDOutput *motor4, bool isRotate);
	virtual ~MultiOutputPID();

	void PIDWrite(float output);

	void SetRotate(bool rot);

	void SetOverDrive(float overDrive);

	void DisableOverDrive();

};

#endif
