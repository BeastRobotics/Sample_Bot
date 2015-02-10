/*
 * CompressorControl.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Beasty
 */
#include "WPILib.h"
#include "IControl.h"

class CompressorControl: public IControl {
	Compressor*c;

public:
	CompressorControl() {
		c = new Compressor(0);
	}
	void RobotInit() {
		c->SetClosedLoopControl(true);
	}

};

