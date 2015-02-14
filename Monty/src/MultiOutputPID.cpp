/*
 * MultiOutputPID.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: David
 */

#include "MultiOutputPID.h"

//For Mechanum assuming (1 and 3 are right) and (2 and 4 are left)
MultiOutputPID::MultiOutputPID(PIDOutput *motor1, PIDOutput *motor2, PIDOutput *motor3, PIDOutput *motor4, bool isRotate) {
	this->motor1=motor1;
	this->motor2=motor2;
	this->motor3=motor3;
	this->motor4=motor4;
	this->isRotate=isRotate;
}

MultiOutputPID::~MultiOutputPID() {
}

void MultiOutputPID::PIDWrite(float output) {
	if (isRotate) {
		motor1->PIDWrite(-output);
		motor2->PIDWrite(output);
		motor3->PIDWrite(-output);
		motor4->PIDWrite(output);
	}
	else {
		motor1->PIDWrite(output);
		motor2->PIDWrite(output);
		motor3->PIDWrite(output);
		motor4->PIDWrite(output);
	}
}
