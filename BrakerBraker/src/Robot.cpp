#include "WPILib.h"
#include "XboxController.h"
#include "PIDLifterControl.h"

class Robot: public IterativeRobot
{


private:

	LiveWindow *lw;
	Encoder *en1;
	Talon *lifter;
	DigitalInput *upperLimit;
	DigitalInput *lowerLimit;
	XboxController *xbox;
	PIDLifterControl *liftControl;
	bool brakeing = false;

public:
	Robot():
		lw(NULL) {
		xbox = XboxController::getInstance();
		en1 = new Encoder(0,1);
		en1->Reset();

		upperLimit = new DigitalInput(4);
		lowerLimit = new DigitalInput(3);
		lifter = new Talon(6);

		liftControl = new PIDLifterControl(en1,lifter);

	}


	void RobotInit()
	{
		lw = LiveWindow::GetInstance();

	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		float speed = 0.0;
		if(xbox->isXHeld() && !upperLimit){
			 if(brakeing){
				 brakeing = false;
				 liftControl->release();
			 }
			 speed = -1.0;
		}
		else if(xbox->isYHeld() && !lowerLimit){
			if(brakeing){
				 brakeing = false;
				liftControl->release();
				}
			speed = 1.0;
		}
		else {
			brakeing = true;
			liftControl->brake();
		}
		lifter->Set(speed);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
