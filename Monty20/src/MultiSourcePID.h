/*
 * MultiSourcePID.h
 *
 *  Created on: Feb 13, 2015
 *      Author: Beasty
 */

#ifndef SRC_MULTISOURCEPID_H_
#define SRC_MULTISOURCEPID_H_

#include "WPILib.h"
#include <PIDSource.h>

class MultiSourcePID: public PIDSource {

	PIDSource *en1;
	PIDSource *en2;
	PIDSource *en3;
	PIDSource *en4;
	PIDSource* encoders[4];

public:
	MultiSourcePID(PIDSource *en1 = NULL, PIDSource *en2 = NULL,
			PIDSource *en3 = NULL, PIDSource *en4 = NULL);
	virtual ~MultiSourcePID();
	double PIDGet();
};

#endif
