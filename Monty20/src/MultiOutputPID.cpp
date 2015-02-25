/*
 * MultiOutputPID.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: David
 */

#include "MultiOutputPID.h"

//For Mechanum assuming (1 and 3 are right) and (2 and 4 are left)
MultiOutputPID::MultiOutputPID(PIDOutput *motor1, PIDOutput *motor2,
		PIDOutput *motor3, PIDOutput *motor4, bool isRotate) {
	this->motor1 = motor1;
	this->motor2 = motor2;
	this->motor3 = motor3;
	this->motor4 = motor4;
	this->isRotate = isRotate;
	overDrive = 0.0;
	overDriveOn = false;
}

MultiOutputPID::~MultiOutputPID() {
}

void MultiOutputPID::PIDWrite(float output) {
	float leftOut;
	float rightOut;

	if (!overDriveOn) {
		leftOut = isRotate ? output : -output;
		rightOut = output;
	} else {
		float offset = output / 2.0;
		bool rotateLeft = output < 0;
		leftOut = -overDrive + (rotateLeft ? offset : -offset);
		rightOut = overDrive + (rotateLeft ? offset : -offset);
	}
	if(motor1!=NULL){
		motor1->PIDWrite(leftOut);
	}
	if(motor2 != NULL){
		motor2->PIDWrite(rightOut);
	}
	if (motor3 != NULL) {
		motor3->PIDWrite(leftOut);
	}
	if (motor4 != NULL) {
		motor4->PIDWrite(rightOut);
	}
}

void MultiOutputPID::SetRotate(bool rot) {
	isRotate = rot;
}

void MultiOutputPID::DisableOverDrive() {
	overDriveOn = false;
	overDrive = 0;
}

void MultiOutputPID::SetOverDrive(float overDrive) {
	overDriveOn = true;
	this->overDrive = overDrive;
}
