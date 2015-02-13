/*
 * Auto.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: Beasty
 */
#include "IControl.h"
#include "LifterControl.cpp"

class AutoBot : public IControl {
	LifterControl lifter;

public:
	AutoBot(IControl *l) {
	 lifter = *(LifterControl*)l;
	}
};

