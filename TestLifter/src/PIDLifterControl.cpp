/*
 * PIDLifterControl.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: education
 */

#include <PIDLifterControl.h>
#include "WPILib.h"


PIDLifterControl::PIDLifterControl(Encoder *nSource, PIDOutput *nOutput){
	//  Auto-generated constructor stub
	this->output = nOutput;
	this->source = nSource;
	p = 0.0;
	i = 0.0;
	d = 0.0;
	controller = new PIDController(p,i,d,source,output);

}

bool PIDLifterControl::start(float np, float ni, float nd){
	//TODO
	controller->SetPID(np,ni,nd);
	controller->Enable();
	if(controller->IsEnabled()) return true;
	else return false;

}

bool PIDLifterControl::disable(){
	controller->Disable();
	return !(controller->IsEnabled());
}

void PIDLifterControl::setPoint(float x = 0){
	controller->SetSetpoint(x);
}

bool PIDLifterControl::brake(){
	return 0;
	//source->reset();
}

PIDLifterControl::~PIDLifterControl() {
	// TODO Auto-generated destructor stub
	this->disable();
	delete controller;
}

