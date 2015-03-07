<<<<<<< HEAD
#include "WPILib.h"
#include "NewXboxController.h"
=======
#include <CameraServer.h>
#include <Compressor.h>
#include <CounterBase.h>
#include <DoubleSolenoid.h>
#include <DriverStation.h>
#include <Encoder.h>
#include <HAL/Task.hpp>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <LiveWindow/LiveWindow.h>
#include <NewXboxController.h>
#include <Preferences.h>
#include <RobotBase.h>
#include <RobotDrive.h>
#include <stddef.h>
#include <SmartDashboard/SmartDashboard.h>
#include <Talon.h>
#include <Task.h>
#include <Timer.h>
#include <XboxController.h>
#include <cstdint>
#include <LifterControl.cpp>

#define TOLERANCE 25
#define LEVEL_1 300
#define MOTOR_SPEED 0.25


>>>>>>> dc6000bd4e802384dc3ed3380389df9a24597f8d

class Robot: public IterativeRobot {

	RobotDrive myRobot; // robot drive system
	LiveWindow *lw;
	Compressor *c = new Compressor(0); //Declasres the compressor
	DoubleSolenoid *ex1 = new DoubleSolenoid(0, 1); //First solenoid pair
	DoubleSolenoid *ex2 = new DoubleSolenoid(2, 3); //Second solenoid pair
	DriverStation *DS = DriverStation::GetInstance(); //Declares the driver station
	Encoder *en1 = new Encoder(1, 0, true, Encoder::k2X); //Motors 1 and 2
	//Encoder *en2 = new Encoder(2, 3, false, Encoder::k2X); //Motors 3 and 4
	CameraServer *c1 = CameraServer::GetInstance(); //Declares camera
	Preferences *pref = Preferences::GetInstance(); //Declares preferences on SmartDashboard
	Task *aTask = NULL;
	Talon *lifterMotor = new Talon(6);
	LifterControl *lifter = new LifterControl();
	int autoLoopCounter;
	bool highGearActivated = false; //False for low gear, true for high gear
	int counter = 0;
	bool run = false;

	NewXboxController* controller=NULL;

public:
	Robot() :
			// No Dash board in Constructo

			myRobot(0,1), // these must be initialized in the same order  2,1 since the turing was inverted
			lw(NULL), autoLoopCounter(0) {
		myRobot.SetExpiration(0.1);
		c1->StartAutomaticCapture();
		lifterMotor->EnableDeadbandElimination(true);
		en1->Reset();
	}

private:
	void RobotInit() {
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit() {
		SmartDashboard::PutString("State", "Autonomous");
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
	static int CountToHundred(uint32_t param)
	{
		Robot *myClass = (Robot*)param;

		for (int i = 0; i < 100 && myClass->run; i++) {
			SmartDashboard::PutNumber("Print Hundred", i + 1);
			Wait(5);
		}
		return 0;
	}

	void TeleopInit() {
		controller=NewXboxController::getInstance();

		SmartDashboard::PutString("State", "Teleop");
		SmartDashboard::PutBoolean("Increment Counter", false);
		//en1->Reset(); //Clears the Encoder
		//en2->Reset(); //Clears the Encoder
		endTask();
		run = true;
		SmartDashboard::PutNumber("Print Hundred",0);
		FUNCPTR myTask = (FUNCPTR)Robot::CountToHundred;
		aTask = new Task("Counter", myTask);
		aTask->Start((uint32_t)this);
	}

	void endTask()
	{
		run = false;
		if (aTask != NULL) {
					bool stopped = aTask->Stop();
					SmartDashboard::PutBoolean("Task Value", stopped);
					SmartDashboard::PutBoolean("Stopped", false);
					//SmartDashboard::PutNumber("Print Hundred",-999);
				}
	}

	void DisabledPeriodic()
	{
		if(aTask!=NULL)
		{
			if(!aTask->Verify())
			{
				delete aTask;
				aTask = NULL;
				SmartDashboard::PutBoolean("Stopped", true);
				SmartDashboard::PutString("State", "Disable Stopped");
			}
		}
	}

	void TeleopPeriodic() {
		if (xbox->isBPressed()) {
			lifter->MoveToLevel1();


		controller->update();

		SmartDashboard::PutBoolean("New Xbox: X", controller->getXHeld());
		SmartDashboard::PutBoolean("New Xbox: Y", controller->getYHeld());
		SmartDashboard::PutBoolean("New Xbox: A", controller->getAHeld());
		SmartDashboard::PutBoolean("New Xbox: B", controller->getBHeld());
		}
		myRobot.ArcadeDrive(xbox->getLeftStick()); //Drives the robot

		SmartDashboard::PutBoolean("High Gear", highGearActivated);
		SmartDashboard::PutNumber("Counter", xbox->getAxisLeftX());
		SmartDashboard::PutNumber("Lifter Encoder", en1->Get()); //X-Value of Joystick

		//Greater
		SmartDashboard::PutString("Greet", "Hello World!");

		//Graph Value
		//SmartDashboard::PutNumber("Right Motor", en1->Get());
		//SmartDashboard::PutNumber("Left Motor", en2->Get());

		//Numerical Value
		//SmartDashboard::PutNumber("Right Motor Count", en1->Get());
		//SmartDashboard::PutNumber("Left Motor Count", en2->Get());

		SmartDashboard::PutNumber("Stick it UP", xbox->getLeftStick()->GetX()); //X-Value of Joystick

		SmartDashboard::PutBoolean("New Xbox: L3", controller->getL3Held());
		SmartDashboard::PutBoolean("New Xbox: R3", controller->getR3Held());
	}

	void DisabledInit()
	{
		SmartDashboard::PutString("State", "Disabled");
		endTask();
	}

	void TestPeriodic() {
		lw->Run();
	}

};

START_ROBOT_CLASS(Robot);
