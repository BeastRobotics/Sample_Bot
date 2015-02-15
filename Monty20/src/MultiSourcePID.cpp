/*
 * MultiSourcePID.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: Beasty
 */

#include <MultiSourcePID.h>

MultiSourcePID::MultiSourcePID(PIDSource *en1, PIDSource *en2, PIDSource *en3, PIDSource *en4) {
	encoders[0] = en1;
	encoders[1] = en2;
	encoders[2] = en3;
	encoders[3] = en4;
}

MultiSourcePID::~MultiSourcePID() {

}

double MultiSourcePID::PIDGet() {
	double average = 0.0;
	int numEncoders = 0;

	for (int i = 0; i < 4; i++) {
		if (encoders[i] != NULL) {
			average += encoders[i]->PIDGet();
			numEncoders++;
		}
	}
	average /= numEncoders;

	return average;
}


