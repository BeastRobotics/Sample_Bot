/*
 * MecanumLiveWindow.cpp
 *
 *  Created on: Feb 17, 2015
 *      Author: Beast Laptop
 */
#include "MecanumDrive.cpp"
#include "WPILib.h"

class MecanumLiveWindow: public MecanumDrive {
	LiveWindow *lw;
public:
	MecanumLiveWindow() :
			lw(NULL) {

	}
private:
	void RobotInit() {
		lw = LiveWindow::GetInstance();
		lw->AddSensor("PID","Gyro", gyro);
	}
};

