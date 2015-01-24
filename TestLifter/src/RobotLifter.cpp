#include "WPILib.h"
#include "XboxController.h"
#include "LifterControl.cpp"
#include <IterativeRobot.h>
#include <RobotBase.h>
#include <RobotDrive.h>

class Robot: public IterativeRobot {

	RobotDrive myRobot;
	XboxController *xbox;
	LiveWindow *lw;
	LifterControl *lifter;
	int autoLoopCounter;
	Talon *lifterMotor;

public:
	Robot() :
			myRobot(0, 1), // these must be initialized in the same order
			lw(NULL), autoLoopCounter(0) {
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance();
		lifter = new LifterControl();
		lifterMotor = new Talon(5);
		lifterMotor->EnableDeadbandElimination(true);
	}

private:
	void RobotInit() {
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit() {
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic() {
		if (autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
				{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
		} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		myRobot.ArcadeDrive(xbox->getLeftStick()); // drive with arcade style (use right stick)

		SmartDashboard::PutNumber("Home Value", 0.0);
		SmartDashboard::PutNumber("Level 1 Value", 0.0);
		SmartDashboard::PutNumber("Level 2 Value", 0.0);
		SmartDashboard::PutNumber("Level 3 Value", 0.0);
		SmartDashboard::PutBoolean("Use Values", false);

		double home = SmartDashboard::GetNumber("HomeValue");
		double level1 = SmartDashboard::GetNumber("Level 1 Value");
		double level2 = SmartDashboard::GetNumber("Level 2 Value");
		double level3 = SmartDashboard::GetNumber("Level 3 Value");

		if (SmartDashboard::GetBoolean("Use Values")) {
			lifter->SetHome((int) home);
			lifter->SetLevel1((int) level1);
			lifter->SetLevel2((int) level2);
			lifter->SetLevel3((int) level3);
		}

	/*
		if (xbox->isBHeld()) {
			lifterMotor->Set(-0.25);
		} else if (xbox->isXHeld()) {
			lifterMotor->Set(0.25);
		} else {
			lifterMotor->Set(0);
		}
	*/

		if (xbox->isBHeld()) {
			lifter->MoveToLevel1();
		} else if (xbox->isYHeld()) {
			lifter->MoveToLevel2();
		} else if (xbox->isXHeld()) {
			lifter->MoveToLevel3();
		} else if (xbox->isAHeld()) {
			lifter->MoveToHome();
		} else {
			lifter->Stop();
		}

	}

	void TestPeriodic() {

		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
