#include <GrabberControl.cpp>
#include "WPILib.h"
#include "XboxController.h"
#include "LifterControl.cpp"
#include <IterativeRobot.h>
#include <RobotBase.h>
#include <RobotDrive.h>
#include <iostream>
#include "IControl.h"

class Robot: public IterativeRobot {

	RobotDrive myRobot;
	XboxController *xbox;
	LiveWindow *lw;
	LifterControl *lifter;
	int autoLoopCounter;
	bool isLifterManual;
	bool isArmManual;
	GrabberControl *grabber;
	Compressor *c;
	CameraServer *c1;
	double home;
	double level1;
	double level2;
	double level3;
public:
	Robot() :
			myRobot(0, 1), // these must be initialized in the same order
			lw(NULL), autoLoopCounter(0) {
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance();
		lifter = new LifterControl();
		isLifterManual = false;
		isArmManual = false;
		grabber = new GrabberControl(xbox->getRightStick());
		c = new Compressor(0);
		c1 = CameraServer::GetInstance();
		c1->StartAutomaticCapture();
		home = 0;
		level1 = 0;
		level2 = 0;
		level3 = 0;
	}

private:
	void RobotInit() {
		lw = LiveWindow::GetInstance();
		//SmartDashboard::PutString("Current Mode", "Robot Init ");

	}

	void AutonomousInit() {
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic() {
		if (autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
				{
			//myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
		} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}
	}

	void TeleopInit() {
		SmartDashboard::PutString("Current Mode", "Init Start");
		SmartDashboard::PutNumber("Set Home Value", 1.0);
		SmartDashboard::PutNumber("Set Level 1 Value", 500.0);
		SmartDashboard::PutNumber("Set Level 2 Value", 1000.0);
		SmartDashboard::PutNumber("Set Level 3 Value", 1500.0);
		SmartDashboard::PutNumber("Current Home Value", 0.0);
		SmartDashboard::PutNumber("Current Level 1 Value", 0.0);
		SmartDashboard::PutNumber("Current Level 2 Value", 0.0);
		SmartDashboard::PutNumber("Current Level 3 Value", 0.0);
		SmartDashboard::PutNumber("Lifter Encoder", 0.0);
		SmartDashboard::PutBoolean("Manual Lifter Mode", true);
		SmartDashboard::PutBoolean("Manual Arm Mode", false);
		SmartDashboard::PutNumber("Left Motor", 0.0);
		SmartDashboard::PutNumber("Right Motor", 0.0);
		SmartDashboard::PutNumber("Grabber Speed Factor", 1.0);
		SmartDashboard::PutNumber("Lifter Speed Factor", 1.0);
		SmartDashboard::PutString("Current Mode", "Init");
		SmartDashboard::PutNumber("Y Axis Right Stick", 0.0);
		SmartDashboard::PutNumber("Lifter Motor Value", 0.0);
		SmartDashboard::PutNumber("Accel Up", 0.1);
		SmartDashboard::PutNumber("Accel Down", 0.1);
		//lifter->SetEncoderValue();

		lifter->Stop();
		SmartDashboard::PutString("Current Mode", "Init Complete");
		std::cout << "init";
	}

	void TeleopPeriodic() {
		SmartDashboard::PutString("Current Mode", "Teleop");
		myRobot.ArcadeDrive(xbox->getLeftStick());
		//drive with arcade style (use right stick)

		SmartDashboard::PutString("Current Mode", "Teleop");

		home = SmartDashboard::GetNumber("Set Home Value");
		level1 = SmartDashboard::GetNumber("Set Level 1 Value");
		level2 = SmartDashboard::GetNumber("Set Level 2 Value");
		level3 = SmartDashboard::GetNumber("Set Level 3 Value");

		lifter->SetHome((int) home);
		lifter->SetLevel1((int) level1);
		lifter->SetLevel2((int) level2);
		lifter->SetLevel3((int) level3);

		SmartDashboard::PutNumber("Current Home Value",
				(double) lifter->homeValue);
		SmartDashboard::PutNumber("Currnet Level 1 Value",
				(double) lifter->level1Value);
		SmartDashboard::PutNumber("Current Level 2 Value",
				(double) lifter->level2Value);
		SmartDashboard::PutNumber("Current Level 3 Value",
				(double) lifter->level3Value);

		SmartDashboard::PutNumber("Lifter Encoder", lifter->GetEnconder());

		SmartDashboard::PutBoolean("Upper Limit", lifter->GetUpperLimit());
		SmartDashboard::PutBoolean("Lower Limit", lifter->GetLowerLimit());

		SmartDashboard::PutNumber("Y Axis Right Stick", xbox->getAxisRightY());
		SmartDashboard::PutNumber("Lifter Motor Value", lifter->lifterSpeed);

		isLifterManual = SmartDashboard::GetBoolean("Manual Lifter Mode");
		isArmManual = SmartDashboard::GetBoolean("Manual Arm Mode");

		SmartDashboard::PutNumber("Left Motor", grabber->leftSpeed);
		SmartDashboard::PutNumber("Right Motor", grabber->rightSpeed);
		grabber->SetSpeeds(SmartDashboard::GetNumber("Grabber Speed Factor"));
		lifter->SetSpeepFactor(
				SmartDashboard::GetNumber("Lifter Speed Factor"));

		if (xbox->isLeftTriggerHeld()) {
			SmartDashboard::PutString("Current Mode", "Lifter Mode");
		} else if (xbox->isRightTriggerHeld()) {
			SmartDashboard::PutString("Current Mode", "Grabber Mode");
		} else {
			SmartDashboard::PutString("Current Mode", "No Mode");
		}

		lifter->SetAccelUp(SmartDashboard::GetNumber("Accel Up"));
		lifter->SetAccelDown(SmartDashboard::GetNumber("Accel Down"));

		if (isLifterManual && !isArmManual) {
			//lifter->ManualMode();

			if (xbox->isBHeld()) {
				lifter->MoveUp();
			} else if (xbox->isXHeld()) {
				lifter->MoveDown();
			} else {
				lifter->Stop();
			}

		} else {
			if (xbox->isLeftTriggerHeld()) {
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
		}
		/*
		if (isArmManual && !isLifterManual) {
			//grabber->DriveWithStick();
		} else {
			/**
			 * A Dive in
			 * B Drive out
			 * X spin clockwise
			 * Y spin counter clockwise

			if (xbox->isRightTriggerHeld()) {
				if (xbox->isAHeld()) {
					grabber->DriveIn();
				} else if (xbox->isBHeld()) {
					SmartDashboard::PutBoolean("Wheels", true);
					grabber->DriveOut();
				} else if (xbox->isXHeld()) {
					SmartDashboard::PutBoolean("Wheels", true);
					grabber->SpinClockWise();
				} else if (xbox->isYHeld()) {
					SmartDashboard::PutBoolean("Wheels", true);
					grabber->SpinCounterClockWise();
				} else {
					grabber->Stop();
				}
			}
		} */

		if (xbox->isRBumperHeld()) {
			grabber->SetGrabberArm(true);
		} else {
			grabber->SetGrabberArm(false);
		}

		grabber->SetMotors();
		lifter->lifterupdate();
	}

	void TestPeriodic() {

		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
