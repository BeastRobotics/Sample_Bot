/*
 * CompressorControl.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Beasty
 */
#include "WPILib.h"
#include "IControl.h"

class CameraControl: public IControl {
	CameraServer *c1;

public:
	CameraControl() {
		c1 = CameraServer::GetInstance();
	}

	void RobotInit() {
		c1->StartAutomaticCapture();
	}
};

