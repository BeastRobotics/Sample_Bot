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

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
