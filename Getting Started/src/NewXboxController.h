/* A new version of XboxController that works on an update system, to be called by a separate thread.
This will attempt to avoid errors in calling certain methods multiple times and getting varying answers.
*/
#ifndef NEWXBOXCONTROLLER_h
#define NEWXBOXCONTROLLER_h
#include "WPILib.h"

class NewXboxController {
public:
	static NewXboxController* getInstance();
	~NewXboxController() {}
	void update();/*MUST BE CALLED EVER ITERATION THROUGH LOOP*/
	
	bool getXPressed();/*PRESSED AND HELD ARE NOT DEPENDENT ON HOW OFTEN THEY ARE CALLED*/
	bool getXHeld();
	bool getYPressed();
	bool getYHeld();
	bool getAPressed();
	bool getAHeld();
	bool getBPressed();
	bool getBHeld();
	bool getStartPressed();
	bool getStartHeld();
	bool getBackPressed();
	bool getBackHeld();
	
	
private:
	NewXboxController(int port=0);
	Joystick lstick;
	Joystick rstick;
	static NewXboxController *newXbox;
	
	bool isButtonHeld(int &debounceCounter, bool rawValue);
	bool xLast, xNow;
	bool yLast, yNow;
	bool aLast, aNow;
	bool bLast, bNow;
	bool startLast, startNow;
	bool backLast, backNow;
	int xDebounceCounter, yDebounceCounter, aDebounceCounter, bDebounceCounter, startDebounceCounter, backDebounceCounter;
};
#endif	
