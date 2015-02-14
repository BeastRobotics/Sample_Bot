/*
 * CompressorControl.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Beasty
 */
#include "WPILib.h"
#include "IControl.h"

class CameraControl: public IControl {

	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;



public:
	CameraControl() {

	}

	void RobotInit() {
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		imaqError = IMAQdxConfigureGrab(session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
		}
		IMAQdxStartAcquisition(session);

	}
	void TeleopPeriodic(){
		IMAQdxGrab(session, frame, true, NULL);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
		} else {
			imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);
		}
	}
};

