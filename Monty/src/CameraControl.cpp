/*
 * CompressorControl.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Beasty
 */
#include "WPILib.h"
#include "IControl.h"

class CameraControl: public IControl {


public:
	CameraControl() {

	}

	void RobotInit() {
		CameraServer::GetInstance()->SetQuality(50);
		//the camera name (ex "cam0") can be found through the roborio web interface
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}
};

