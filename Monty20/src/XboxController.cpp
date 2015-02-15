/*
 * XboxController.cpp
 *
 *  Created on: Jan 7, 2015
 *      Author: Beasty
 */


/* This is the CPP file for Xbox Controller */

#include "WPILib.h"
#include "XboxController.h"
#include "NewXboxController.h"
#include <cmath>
/* These are the button mappings to the raw values on the xbox controller*/

XboxController *XboxController::getInstance() {
	return NewXboxController::getInstance();
}
#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 3
#define BUTTON_Y 4
#define BUTTON_LB 5
#define BUTTON_RB 6
#define BUTTON_BACK 7
#define BUTTON_START 8
#define BUTTON_L3 9 // Press down the left joystick for L3.
#define BUTTON_R3 10 // Press down the right joystick for R3.
#define AXIS_RIGHT_X

#define AXIS_RIGHT_Y 2
#define AXIS_LEFT_X 4
#define AXIS_LEFT_Y 5
#define AXIS_TRIGGER 3
#define JOG_DEBOUNCE 10
// Number of consecutive hits to count as pressed.
/* The constructor is fed the number for initializing the Joysticks*/

XboxController::XboxController(int stick) :
	lstick(stick), rstick(stick) {
	rstick.SetAxisChannel(Joystick::kXAxis, 4);
	rstick.SetAxisChannel(Joystick::kYAxis, 5);
	a = 0;
	b = 0;
	x = 0;
	y = 0;
	lb = 0;
	rb = 0;
	back = 0;
	start = 0;
	l3 = 0;
	r3 = 0;
	leftJog = 0;
	upJog = 0;
	downJog = 0;
	rightJog = 0;
	rightTrigger = 0;
	leftTrigger = 0;
	endGame=0;
}
bool XboxController::isLeftTriggerHeld() {
	bool isPressed = false;
	if (this->getAxisTrigger() >= .8) {
		isPressed = true;
	}
	return isButtonHeld(leftTrigger, isPressed, JOG_DEBOUNCE);
}
bool XboxController::isRightTriggerHeld() {
	bool isPressed = false;
	if (this->getAxisTrigger() <= -.8) {
		isPressed = true;
	}
	return isButtonHeld(rightTrigger, isPressed, JOG_DEBOUNCE);
}

bool XboxController::isRightTriggerPressed(){
	bool isPressed = false;
	if(this->getAxisTrigger() <= -.3){
		isPressed = true;
	}
	return isPressed;
}

bool XboxController::JogEnabled() {
	return (this->getAxisLeftX() >= .8) && (fabs(this->getAxisLeftY()) <= .16);
}

/* Returns true if activated*/
bool XboxController::isLeftJogPressed() {
	if (JogEnabled()) {
		if ((this->getAxisRightX() <= -.8) && (fabs(this->getAxisRightY())
				<= .16)) {
			return isButtonPressed(leftJog, true, JOG_DEBOUNCE);

		}
	}
	return isButtonPressed(leftJog, false, JOG_DEBOUNCE);
}
bool XboxController::isUpJogPressed() {
	if (JogEnabled()) {
		if ((this->getAxisRightY() >= .8) && (fabs(this->getAxisRightX())
				<= .16)) {
			return isButtonPressed(upJog, true, JOG_DEBOUNCE);

		}
	}
	return isButtonPressed(upJog, false, JOG_DEBOUNCE);
}
bool XboxController::isDownJogPressed() {
	if (JogEnabled()) {
		if ((this->getAxisRightY() <= -.8) && (fabs(this->getAxisRightX())
				<= .16)) {
			return isButtonPressed(downJog, true, JOG_DEBOUNCE);

		}
	}
	return isButtonPressed(downJog, false, JOG_DEBOUNCE);
}
bool XboxController::isRightJogPressed() {
	if (JogEnabled()) {
		if ((this->getAxisRightX() >= .8) && (fabs(this->getAxisRightY())
				<= .16)) {
			return isButtonPressed(rightJog, true, JOG_DEBOUNCE);

		}
	}
	return isButtonPressed(rightJog, false, JOG_DEBOUNCE);
}
bool XboxController::isAPressed() {
	return isButtonPressed(a, rstick.GetRawButton(BUTTON_A));
}

bool XboxController::isBPressed() {
	return isButtonPressed(b, rstick.GetRawButton(BUTTON_B));
}

bool XboxController::isXPressed() {
	return isButtonPressed(x, rstick.GetRawButton(BUTTON_X));
}

bool XboxController::isYPressed() {
	return isButtonPressed(y, rstick.GetRawButton(BUTTON_Y));
}

bool XboxController::isLBumperPressed() {
	return isButtonPressed(lb, rstick.GetRawButton(BUTTON_LB));
}

bool XboxController::isRBumperPressed() {
	return isButtonPressed(rb, rstick.GetRawButton(BUTTON_RB));
}

bool XboxController::isBackPressed() {
	return isButtonPressed(back, rstick.GetRawButton(BUTTON_BACK));
}

bool XboxController::isStartPressed() {
	return isButtonPressed(start, rstick.GetRawButton(BUTTON_START));
}

bool XboxController::isL3Pressed() {
	return isButtonPressed(l3, rstick.GetRawButton(BUTTON_L3));
}

bool XboxController::isR3Pressed() {
	return isButtonPressed(r3, rstick.GetRawButton(BUTTON_R3));
}

bool XboxController::isAHeld() {
	return isButtonHeld(a, rstick.GetRawButton(BUTTON_A));
}

bool XboxController::isBHeld() {
	return isButtonHeld(b, rstick.GetRawButton(BUTTON_B));
}

bool XboxController::isXHeld() {
	return isButtonHeld(x, rstick.GetRawButton(BUTTON_X));
}

bool XboxController::isYHeld() {
	return isButtonHeld(y, rstick.GetRawButton(BUTTON_Y));
}

bool XboxController::isLBumperHeld() {
	return isButtonHeld(lb, rstick.GetRawButton(BUTTON_LB));
}

bool XboxController::isRBumperHeld() {
	return isButtonHeld(rb, rstick.GetRawButton(BUTTON_RB));
}

bool XboxController::isBackHeld() {
	return isButtonHeld(back, rstick.GetRawButton(BUTTON_BACK));
}

bool XboxController::isStartHeld() {
	return isButtonHeld(start, rstick.GetRawButton(BUTTON_START));
}

bool XboxController::isL3Held() {
	return isButtonHeld(l3, rstick.GetRawButton(BUTTON_L3));
}

bool XboxController::isR3Held() {
	return isButtonHeld(r3, rstick.GetRawButton(BUTTON_R3));
}
/* The Raw Axis values are inverted so that they make sense. Up is positive now. */
float XboxController::getAxisRightX() {
	return (-1.0) * rstick.GetRawAxis(4);
}

float XboxController::getAxisRightY() {
	return (-1.0) * rstick.GetRawAxis(5);
}

float XboxController::getAxisLeftX() {
	return (-1.0) * lstick.GetRawAxis(0);
}

float XboxController::getAxisLeftY() {
	return (-1.0) * lstick.GetRawAxis(1);
}

/* Remeber that the right and left trigger make up one axis total. */
float XboxController::getAxisTrigger() {
	return rstick.GetRawAxis(3);
}
bool XboxController::isButtonPressed(int &counter, bool isRawPressed,
		int debouncedCount) {
	if (isRawPressed) {
		// This debounces the button
		counter++;
		// Button must be recounterd counters pressed 60 times, then toggle
		if (counter == debouncedCount) {
			return true;
		}
	} else {
		// If button not pressed, counter reset.
		counter = 0;
	}
	return false;
}
bool XboxController::isButtonPressed(int &counter, bool rawValue) {
	return isButtonPressed(counter, rawValue, DEBOUNCER);
}
bool XboxController::isButtonHeld(int &counter, bool isRawPressed) {
	return isButtonHeld(counter, isRawPressed, DEBOUNCER);
}
bool XboxController::isButtonHeld(int &counter, bool rawValue,
		int debounceCount) {
	if (rawValue) {
		// This debounces the button
		counter++;
		// Button must be recounterd counters pressed 60 times, then toggle
		if (counter > debounceCount) {
			return true;
		}
	} else {
		// If button not pressed, counter reset.
		counter = 0;
	}
	return false;
}
//start and y
bool XboxController::isEndGame(){
	return isButtonPressed(endGame, rstick.GetRawButton(BUTTON_START) && rstick.GetRawButton(BUTTON_Y)) ;

}

Joystick* XboxController::getLeftStick() {
	return &lstick;
}
Joystick* XboxController::getRightStick() {
	return &rstick;
}


