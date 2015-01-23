#include "WPILib.h"
#include "XboxController.h"
#include "LifterControl.cpp"
#include <IterativeRobot.h>
#include <RobotBase.h>
#include <RobotDrive.h>

class Robot: public IterativeRobot
{

	RobotDrive myRobot;
	XboxController *xbox;
	LiveWindow *lw;
	LifterControl *lifter;
	int autoLoopCounter;
	Talon *lifterMotor;

public:
	Robot() :
		myRobot(0,1), // these must be initialized in the same order
		lw(NULL),
		autoLoopCounter(0)
	{
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance();
		lifter = new LifterControl();
		lifterMotor->EnableDeadbandElimination(true);
		lifterMotor = new Talon(6);
	}


private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{
		if(autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
			} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(xbox->getLeftStick()); // drive with arcade style (use right stick)

		if (xbox->isBPressed()) {
			lifterMotor->Set(-0.25);
		} else if (xbox->isXPressed()) {
			lifterMotor->Set(0.25);
		} else {
			lifterMotor->Set(0);
		}
	}

	void TestPeriodic()
	{

		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
