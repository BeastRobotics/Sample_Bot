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
	MultiOutputPID(PIDOutput *motor1=NULL, PIDOutput *motor2=NULL, PIDOutput *motor3=NULL, PIDOutput *motor4=NULL, bool isRotate=false);
	virtual ~MultiOutputPID();

	void PIDWrite(float output);

	void SetRotate(bool rot);

	void SetOverDrive(float overDrive);

	void DisableOverDrive();

};

#endif
