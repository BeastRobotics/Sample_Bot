/*
 * PIDTest.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: Daniel
 */

#include <PIDTest.h>

PIDTest::PIDTest() {
	// TODO Auto-generated constructor stub

}

PIDTest::~PIDTest() {
	// TODO Auto-generated destructor stub
}

void PIDTest::PIDInit(){

}

int PIDTest::PIDControl(int kp,int ki,int kd,int input) {

	while(target != value){
		value=input;
		error=value-target;
		powerAdjust=error*kp;

	}
	return output;
}
