/*
 * PIDTest.h
 *
 *  Created on: Feb 15, 2015
 *      Author: Daniel
 */

#ifndef SRC_PIDTEST_H_
#define SRC_PIDTEST_H_

class PIDTest {

	int kp;
	int ki;
	int kd;
	int target;
	int error;
	int power;
	int input;
	int output;
	int value;
	int powerAdjust;

	bool direction;
public:
	PIDTest();
	virtual ~PIDTest();
	void PIDInit();
	int PIDControl(int kp,int ki,int kd,int input);

};

#endif /* SRC_PIDTEST_H_ */
