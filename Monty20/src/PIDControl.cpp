/*
 * PIDControl.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: education
 */

#include "PIDControl.h"
#include "WPILib.h"


PIDControl::PIDControl(Encoder *nSource, PIDOutput *nOutput){
	//  Auto-generated constructor stub
	this->output = nOutput;
	this->source = nSource;
	p = 0.0;
	i = 0.0;
	d = 0.0;
	controller = new PIDController(p,i,d,source,output);

}

bool PIDControl::start(float np, float ni, float nd){
	//TODO
	controller->SetPID(np,ni,nd);
	controller->Enable();
	return controller->IsEnabled();
}

bool PIDControl::disable(){
	controller->Disable();
	return !(controller->IsEnabled());
}

void PIDControl::setPoint(float x){
	controller->SetSetpoint(x);
}

bool PIDControl::brake(float np, float ni, float nd){
	if(!(controller->IsEnabled())){
		source->Reset();
		this->setPoint();
		return this->start(np, ni, nd);
	}
	else return true;
}

bool PIDControl::release(){
	return this->disable();
}

PIDControl::~PIDControl() {
	// TODO Auto-generated destructor stub
	this->disable();
	delete controller;
}
