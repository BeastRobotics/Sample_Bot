/*
 * LifterBrake.cpp
 *
 *  Created on: Feb 11, 2015
 *      Author: Beasty
 */

#include <LifterBrake.h>


LifterBrake::LifterBrake() {
	en1 = new Encoder(0, 1, true);
	en1->Reset();
	pidControl = new PIDControl(this->en1, this->lifter);
	pref = Preferences::GetInstance();
	p = i = d = 0.0;

}

LifterBrake::~LifterBrake() {
	pidControl->release();
	delete pidControl;
}

void LifterBrake::brake() {
	p = pref->GetDouble("P_Value");
	i = pref->GetDouble("I_Value");
	d = pref->GetDouble("D_Value");
	pidControl->brake(p, i, d);
}

void LifterBrake::release() {
	pidControl->release();
}

void LifterBrake::TeleopPeriodic() {
	LifterControl::TeleopPeriodic();
	SmartDashboard::PutNumber("P Value", p);
	SmartDashboard::PutNumber("I Value", i);
	SmartDashboard::PutNumber("D Value", d);
	SmartDashboard::PutNumber("Lifter Encoder", GetEncoder());
}

int LifterBrake::GetEncoder() {
	return en1->Get();
}
