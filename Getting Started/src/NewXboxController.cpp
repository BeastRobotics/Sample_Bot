/* A new version of XboxController that workes on an update system, to be called by a separate thread.
This will attempt to avoid errors in calling certain methods multiple times and getting varying answers.
*/

#include "WPILib.h"
#include "Nivision.h"
#include "NewXboxController.h"
#include <cmath>

#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 3
#define BUTTON_Y 4
#define BUTTON_LB 5
#define BUTTON_RB 6
#define BUTTON_BACK 7
#define BUTTON_START 8
#define BUTTON_L3 9 // Press down the left joystick for L3.
#define BUTTON_R3 10 // Press down the right joystick for R3. David is stupid.
#define AXIS_RIGHT_X 1
#define AXIS_RIGHT_Y 2
#define AXIS_LEFT_X 4
#define AXIS_LEFT_Y 5
#define AXIS_TRIGGER 3
#define JOG_DEBOUNCE 10

#define DEBOUNCE_COUNT_LIMIT 15
	
NewXboxController::NewXboxController(int port):
		lstick(port), rstick(port) {
	rstick.SetAxisChannel(Joystick::kXAxis, 4);
	rstick.SetAxisChannel(Joystick::kYAxis, 5);
	//set all buttons to false
	xLast=false;
	xNow=false;
	yLast=false;
	yNow=false;
	aLast=false;
	aNow=false;
	bLast=false;
	bNow=false;
	startLast=false;
	startNow=false;
	backLast=false;
	backNow=false;
	
	//set counters to 0
	xDebounceCounter=0;
	yDebounceCounter=0;
	aDebounceCounter=0;
	bDebounceCounter=0;
	startDebounceCounter=0;
	backDebounceCounter=0;
}
	
NewXboxController *NewXboxController::getInstance() {
	if (newXbox == NULL)
		newXbox = new NewXboxController(0);//this is the first time getInstance is called
		
	return newXbox;
}
	
void NewXboxController::update() {
	xLast=xNow;
	yLast=yNow;
	aLast=aNow;
	bLast=bNow;
	startLast=startNow;
	backLast=backNow;
	
	xNow=isButtonHeld(xDebounceCounter, rstick.GetRawButton(BUTTON_X));
	yNow=isButtonHeld(yDebounceCounter, rstick.GetRawButton(BUTTON_Y));
	aNow=isButtonHeld(aDebounceCounter, rstick.GetRawButton(BUTTON_A));
	bNow=isButtonHeld(bDebounceCounter, rstick.GetRawButton(BUTTON_B));
	startNow=isButtonHeld(startDebounceCounter, rstick.GetRawButton(BUTTON_START));
	backNow=isButtonHeld(backDebounceCounter, rstick.GetRawButton(BUTTON_BACK));
}

bool NewXboxController::getXPressed() {
	return (!xLast)&&(xNow);
}

bool NewXboxController::getYPressed() {
	return (!yLast)&&(yNow);
}

bool NewXboxController::getAPressed() {
	return (!aLast)&&(aNow);
}

bool NewXboxController::getBPressed() {
	return (!bLast)&&(bNow);
}

bool NewXboxController::getXHeld() {
	return xLast&&xNow;
}

bool NewXboxController::getYHeld() {
	return yLast&&yNow;
}

bool NewXboxController::getAHeld() {
	return aLast&&aNow;
}

bool NewXboxController::getBHeld() {
	return bLast&&bNow;
}

bool NewXboxController::getStartHeld() {
	return startLast&&startNow;
}

bool NewXboxController::isButtonHeld(int &debounceCounter, bool rawValue) 
	if (rawValue) {
		debounceCounter++;
		if (debounceCounter > DEBOUNCE_COUNT_LIMIT) return true;
	} 
	else {//we are still debouncing...
		debounceCounter = 0;
	}
	return false;
}
