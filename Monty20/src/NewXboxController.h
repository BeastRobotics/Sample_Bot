/* A new version of XboxController that works on an update system, to be called by a separate thread.
 This will attempt to avoid errors in calling certain methods multiple times and getting varying answers.
 */
#ifndef NEWXBOXCONTROLLER_h
#define NEWXBOXCONTROLLER_h
#include "WPILib.h"
#include "RestrictedXboxController.h"
#include "XboxController.h"
#include "IControl.h"

class NewXboxController: public RestrictedXboxController, public XboxController, public IControl {
public:
	static NewXboxController* getInstance(int port=0);
	~NewXboxController() {
	}
	Joystick* getLeftStick() {
		return &lstick;
	}
	Joystick* getRightStick() {
		return &rstick;
	}

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

	bool getLeftBumperPressed();
	bool getLeftBumperHeld();
	bool getRightBumperPressed();
	bool getRightBumperHeld();
	bool getR3Pressed();
	bool getR3Held();
	bool getL3Pressed();
	bool getL3Held();

	bool getLeftTriggerPressed();
	bool getLeftTriggerHeld();
	bool getRightTriggerPressed();
	bool getRightTriggerHeld();

	float getAxisLeftX();
	float getAxisLeftY();
	float getAxisRightX();
	float getAxisRightY();

private:
	NewXboxController(int port = 0);
	Joystick lstick;
	Joystick rstick;

	Timer *timer;
	double lostTimeBank, lastTime;
	void updateAllCounters();
	void accountForLostTime();

	bool isButtonHeld(int &debounceCounter, bool rawValue);
	float getAxisTriggerLeft();
	float getAxisTriggerRight();
	bool xLast, xNow;
	bool yLast, yNow;
	bool aLast, aNow;
	bool bLast, bNow;
	bool startLast, startNow;
	bool backLast, backNow;
	bool leftBumperNow, leftBumperLast;
	bool rightBumperNow, rightBumperLast;
	bool l3Last, l3Now;
	bool r3Last, r3Now;
	bool leftTriggerLast, leftTriggerNow;
	bool rightTriggerLast, rightTriggerNow;

	int xDebounceCounter, yDebounceCounter, aDebounceCounter, bDebounceCounter,
			startDebounceCounter, backDebounceCounter;
	int leftBumperDebounceCounter, rightBumperDebounceCounter,
			r3DebounceCounter, l3DebounceCounter;
	int leftTriggerDebounceCounter, rightTriggerDebounceCounter;

public: //Stuff from Xbox Controller
	bool isLeftJogPressed();
	bool isRightJogPressed();
	bool isUpJogPressed();
	bool isDownJogPressed();
	bool isAPressed() {return getAPressed();}
	bool isBPressed() {return getBPressed();}
	bool isXPressed() {return getXPressed();}
	bool isYPressed() {return getYPressed();}
	bool isLBumperPressed() {return getLeftBumperPressed();}
	bool isRBumperPressed() {return getRightBumperPressed();}
	bool isBackPressed() {return getBackPressed();}
	bool isStartPressed() {return getStartPressed();}
	bool isL3Pressed() {return getL3Pressed();}
	bool isR3Pressed() {return getR3Pressed();}
	bool isAHeld() {return getAHeld();}
	bool isBHeld() {return getBHeld();}
	bool isXHeld() {return getXHeld();}
	bool isYHeld() {return getYHeld();}
	bool isLBumperHeld() {return getLeftBumperHeld();}
	bool isRBumperHeld() {return getRightBumperHeld();}
	bool isBackHeld() {return getBackHeld();}
	bool isStartHeld() {return getStartHeld();}
	bool isL3Held() {return getL3Held();}
	bool isR3Held() {return getR3Held();}
	bool isRightTriggerHeld() {return getRightTriggerHeld();}
	bool isRightTriggerPressed() {return getRightTriggerPressed();}
	bool isLeftTriggerHeld() {return getLeftTriggerHeld();}

	//stuff from IControl
	void TeleopPeriodic() {update();}
};
#endif	
